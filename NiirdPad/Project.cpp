#include "Project.h"

#include <experimental\filesystem>
#include <fstream>

#include <rapidjson\document.h>
#include <rapidjson\rapidjson.h>
#include <rapidjson\istreamwrapper.h>
#include <rapidjson\ostreamwrapper.h>
//#include <rapidjson\stringbuffer.h>
#include <rapidjson\writer.h>
#include <QFileDialog>

#include "Character.h"
#include "DialogueFile.h"
#include "NiirdPad.h"
#include "Node.h"
#include "RawProjectFile.h"

//Project::Project(QNodeView &NodeView, const std::string &Path) :
//	Project(NodeView)
//{
//	Load(Path);
//}
//
//Project::Project(QNodeView &NodeView, const RawProjectFile &ImportedProject) :
//	Project(NodeView)
//{
//	Load(ImportedProject);
//}

Project::Project(QNodeView &NodeView) :
	_NodeView(NodeView),
	_ProjectSchema(nullptr)
{
	QFile SchemaFile(":/NiirdPad/Resources/Schema.json");
	if (SchemaFile.open(QIODevice::OpenModeFlag::ReadOnly))
	{
		std::string SchemaStr = SchemaFile.readAll().toStdString();
		if (SchemaStr.length() == 0)
			fmt::print("Unable to load schema file '{}'.", SchemaFile.fileName().toStdString());

		rapidjson::StringStream InStringStream(SchemaStr.c_str());

		rapidjson::Document SchemaDoc;
		SchemaDoc.ParseStream(InStringStream);

		_ProjectSchema = new rapidjson::SchemaDocument(SchemaDoc);
	}
	else
	{
		fmt::print("Unable to open file '{}'.", SchemaFile.fileName().toStdString());
	}
}

Project::~Project()
{
	Unload();
}

void Project::New()
{
	Unload();
	// TODO: Check existing characters and make a new name if necessary / prompt the user for a name
	Character *NewChar = NewCharacter("New Character"); // Legally distinct from Niirds™

	DialogueFile *NewDiag = NewChar->NewDialogueFile("diag");
}

const std::string &Project::SavePath() const
{
	return _savePath;
}

bool Project::Save()
{
	return SaveAs(_savePath);
}

bool Project::SaveAs(const std::string &Path)
{
	std::string DestPath = Path;

	if (DestPath == "" || !std::experimental::filesystem::exists(DestPath))
	{
		// Have the user pick a destination here.
		DestPath = QFileDialog::getSaveFileName(this->_NodeView.GetNiirdPad(), "Save Project As", QDir::homePath(), "NiirdPad Project File (*.niirdproj)").toStdString();
		if(DestPath == "")
			return false;
	}

	rapidjson::Document Doc;
	Doc.SetObject();

	// TODO: Implement project comment.
	rapidjson::Value Comment("", 0);
	Doc.AddMember("comment", Comment, Doc.GetAllocator());

	// TODO: Ensure files with a blank version string are upgraded to the currently-loaded dataset's version.
	const std::string &VersionString = _NodeView.GetNiirdPad()->ScriptEngine().VersionString();
	rapidjson::Value Version(VersionString.c_str(), VersionString.length(), Doc.GetAllocator());
	Doc.AddMember("version", Version, Doc.GetAllocator());

	rapidjson::Value Characters(rapidjson::kArrayType);
	for (auto CurChar : _Characters)
	{
		rapidjson::Value NewChar(rapidjson::kObjectType);
		CurChar.second->Save(Doc, NewChar);
		Characters.PushBack(NewChar, Doc.GetAllocator());
	}
	Doc.AddMember("characters", Characters, Doc.GetAllocator());

	std::ofstream OutStream(DestPath);
	rapidjson::OStreamWrapper OutStreamWrapper(OutStream);
	rapidjson::Writer<rapidjson::OStreamWrapper> Writer(OutStreamWrapper);

	bool bSuccess = Doc.Accept(Writer);
	if (bSuccess)
	{
		_savePath = DestPath;
		CleanChanges();
	}

	return bSuccess;
}

#undef GetObject

bool Project::Load(const std::string &Path, std::string *ErrorMessage)
{
	Unload();

	if (_ProjectSchema == nullptr)
	{
		if (ErrorMessage)	*ErrorMessage = "No schema file loaded.";
		return false;
	}

	std::string LoadPath = Path;
	if (LoadPath == "")
	{
		LoadPath = QFileDialog::getOpenFileName(this->_NodeView.GetNiirdPad(), "Open Project", QDir::homePath(), "NiirdPad Project File (*.niirdproj)").toStdString();
		if (LoadPath == "")
			return false;

		if (!std::experimental::filesystem::exists(LoadPath))
		{
			if (ErrorMessage)	*ErrorMessage = "Specified file does not exist.";
			return false;
		}
	}

	std::ifstream InStream(LoadPath);
	rapidjson::IStreamWrapper InStreamWrapper(InStream);

	rapidjson::Document Doc;
	Doc.ParseStream(InStreamWrapper);
	if (Doc.HasParseError())
	{
		if (ErrorMessage)	*ErrorMessage = RAPIDJSON_ERROR_STRING(Doc.GetParseError());
		return false;
	}
	
	rapidjson::SchemaValidator Validator(*_ProjectSchema);
	if (!Doc.Accept(Validator))
	{
		rapidjson::StringBuffer sb;
		Validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
		std::string UriFrag = sb.GetString();
		sb.Clear();
		Validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
		std::string DocPtr = sb.GetString();
		if (ErrorMessage)	*ErrorMessage = fmt::format("Invalid schema: {}\nInvalid keyword: {}\nInvalid document: {}", UriFrag, Validator.GetInvalidSchemaKeyword(), DocPtr);
		return false;
	}

	if (_NodeView.GetNiirdPad()->ScriptEngine().VersionString() != Doc["version"].GetString())
	{
		if (ErrorMessage)	*ErrorMessage = "Version string mismatch. File: " + std::string(Doc["version"].GetString()) + ", NiirdPad: " + _NodeView.GetNiirdPad()->ScriptEngine().VersionString();
		return false;
	}

	_savePath = LoadPath;
	_bUnsavedChanges = false;

	rapidjson::Value Characters = Doc["characters"].GetArray();
	for (auto &CurChar : Characters.GetArray())
	{
		std::string NewCharName = CurChar["name"].GetString();
		Character *NewChar = NewCharacter(NewCharName);
		// TODO: Character comments
		for (auto &CurDiagFile : CurChar["files"].GetArray())
		{
			std::vector<std::tuple<NodeOption*, std::string, std::string>> PendingConnections;
			// <0> - Source pointer
			// <1> - Connection key name
			// <2> - Destination index

			std::string NewDiagFileName = CurDiagFile["filename"].GetString();
			DialogueFile *NewDiagFile = NewChar->NewDialogueFile(NewDiagFileName);

			SDL_Point CameraPos = { CurDiagFile["cameraPos"]["x"].GetInt(), CurDiagFile["cameraPos"]["y"].GetInt() };
			// TODO: Implement per-dialogue file camera positions
			this->_NodeView.GetCamera().ViewBox.x = CameraPos.x;
			this->_NodeView.GetCamera().ViewBox.y = CameraPos.y;

			// TODO: Dialogue file comments
			for (auto &CurNode : CurDiagFile["nodes"].GetArray())
			{
				SDL_Point NewNodePos = { CurNode["scenePosition"]["x"].GetInt(), CurNode["scenePosition"]["y"].GetInt() };
				Node *NewNode = NewDiagFile->NewNode(NewNodePos);
				NewNode->SetComment(CurNode["comment"].GetString());
				
				for (auto &CurDlg : CurNode["dialogues"].GetArray())
				{
					std::string NewDlgIndex = CurDlg["index"].GetString();
					NodeDialogue *NewDlg = NewNode->AddDialogue(NewDlgIndex);
					
					auto NewDlgWindowState = QByteArray::fromBase64(QByteArray(CurDlg["editorWindowState"].GetString()));
					NewDlg->SetWindowState(NewDlgWindowState);

					std::string NewDlgText = CurDlg["text"].GetString();
					std::vector<std::string> NewDlgFuncLines;
					for (auto &CurFuncLine : CurDlg["functionLines"].GetArray())
					{
						NewDlgFuncLines.push_back(CurFuncLine.GetString());
					}

					NewDlg->SetAll(NewDlgFuncLines, NewDlgText);
				}

				for (auto &CurOpt : CurNode["options"].GetArray())
				{
					NodeOption *NewOpt = NewNode->AddOption();

					auto NewOptWindowState = QByteArray::fromBase64(QByteArray(CurOpt["editorWindowState"].GetString()));
					NewOpt->SetWindowState(NewOptWindowState);

					std::string NewOptText = CurOpt["text"].GetString();
					std::vector<std::string> NewOptFuncLines, NewOptVisLines;
					for (auto &CurFuncLine : CurOpt["functionLines"].GetArray())
					{
						NewOptFuncLines.push_back(CurFuncLine.GetString());
					}
					for (auto &CurVisLine : CurOpt["visibilityLines"].GetArray())
					{
						NewOptVisLines.push_back(CurVisLine.GetString());
					}

					NewOpt->SetAll(NewOptVisLines, NewOptFuncLines, NewOptText);

					for (auto &CurConn : CurOpt["connections"].GetArray())
					{
						std::string KeyName = CurConn["branchOption"].GetString();
						std::string DestIndex = CurConn["destIndex"].GetString();

						auto NewTuple = std::make_tuple(NewOpt, KeyName, DestIndex);
						PendingConnections.push_back(NewTuple);
					}
				}
			}

			// Connect PendingConnections here.
			for (auto &CurConn : PendingConnections)
			{
				NodeOption *SrcOpt = std::get<0>(CurConn);
				std::string KeyName = std::get<1>(CurConn);
				std::string DestIndex = std::get<2>(CurConn);

				// Skip keys that aren't connected to anything.
				if (DestIndex == "")
					continue;

				auto &OptConns = SrcOpt->Nub().Connections();
				auto OutConnRes = std::find_if(OptConns.begin(), OptConns.end(), [KeyName](ConnectionOutput *Conn) {
					return Conn->KeyName() == KeyName;
				});
				if (OutConnRes == OptConns.end())
				{
					// Something's gone dreadfully, horribly, disgustingly wrong.
					if (ErrorMessage != nullptr)	*ErrorMessage = "Attempted to connect non-existent key '" + KeyName + "' to index '" + DestIndex + "'.";
					return false;
				}

				ConnectionOutput *SrcConnection = *OutConnRes;

				// Now find the destination
				auto DiagNodes = NewDiagFile->GetNodes();
				auto DestNodeRes = std::find_if(DiagNodes.begin(), DiagNodes.end(), [DestIndex](Node *Nd) {
					return Nd->HasIndex(DestIndex);
				});
				if (DestNodeRes == DiagNodes.end())
				{
					// Something ELSE has gone dreadfully, horribly, disgustingly wrong.
					if (ErrorMessage != nullptr)	*ErrorMessage = "Attempted to connect key '" + KeyName + "' to non-existent index '" + DestIndex + "'.";
					return false;
				}

				auto &DestNodeIndices = (*DestNodeRes)->Dialogues();
				auto DestConnRes = std::find_if(DestNodeIndices.begin(), DestNodeIndices.end(), [DestIndex](NodeDialogue *NdDlg) {
					return (NdDlg->GetReference() == DestIndex);
				});
				if (DestConnRes == DestNodeIndices.end())
				{
					// Something ELSE ELSE!!! has gone dreadfully, horribly, disgustingly wrong.
					if (ErrorMessage != nullptr)	*ErrorMessage = "Attempted to connect key '" + KeyName + "' to non-existent index '" + DestIndex + "'.";
					return false;
				}

				ConnectionInput &DestConnection = (*DestConnRes)->Nub().Connection();

				// Finally connect the fucking thing
				SrcConnection->Connect(&DestConnection);
			}
		}
	}

	return true;
}

bool Project::Load(const RawProjectFile &ImportedProject, std::string *ErrorMessage)
{
	// TODO: Finish implementing.
	//return false;	// Just so if it is called, nothing will happen.

	Unload();

	// TODO: Replace CurrentOffset with a proper layout algorithm.
	for (auto &CurrentCharacter : ImportedProject.Characters())
	{
		const std::string &CharacterName = CurrentCharacter.first;
		const auto &CharacterData = CurrentCharacter.second;

		Character *NewChar = NewCharacter(CharacterName);
		for (auto &CurrentFile : CharacterData.Files())
		{
			std::vector<std::tuple<NodeOption*, std::string, std::string>> PendingConnections;
			// <0> - Source pointer
			// <1> - Connection key name
			// <2> - Destination index

			int CurrentOffset = 0;
			const std::string &FileName = CurrentFile.first;
			auto &FileData = CurrentFile.second;

			DialogueFile *NewFile = NewChar->NewDialogueFile(FileName);
			for (auto &CurrentNode : FileData.Nodes())
			{
				Node *NewNode = NewFile->NewNode();
				// HACK: Right here, see TODO above.
				NewNode->SetPosition({ CurrentOffset, 0 });
				CurrentOffset += 350;

				const std::string &Comment = CurrentNode.Comment();
				NewNode->SetComment(Comment);

				for (auto &CurrentDialogue : CurrentNode.Dialogues())
				{
					auto NewDlg = NewNode->AddDialogue(CurrentDialogue.Reference());
					NewDlg->SetAll(CurrentDialogue.Scripts(), CurrentDialogue.Dialogue());
				}

				for (auto &CurrentOption : CurrentNode.Options())
				{
					auto NewOpt = NewNode->AddOption();
					NewOpt->SetAll(CurrentOption.VisibilityScripts(), CurrentOption.Functions(), CurrentOption.Text());

					//CurrentFile.second.Nodes()[0].Dialogues()[0].
					std::string KeyName = CurConn["branchOption"].GetString();
					std::string DestIndex = CurConn["destIndex"].GetString();

					auto NewTuple = std::make_tuple(NewOpt, KeyName, DestIndex);
					PendingConnections.push_back(NewTuple);
				}
			}
		}
	}

	return true;
}

void Project::Unload()
{
	_savePath = "";
	_bUnsavedChanges = false;

	for (auto CurChar : _Characters)
	{
		Character *Ptr = CurChar.second;
		delete Ptr;
	}
	_Characters.clear();
}

bool Project::UnsavedChanges() const
{
	return _bUnsavedChanges;
}

void Project::DirtyChanges()
{
	_bUnsavedChanges = true;
}

void Project::CleanChanges()
{
	_bUnsavedChanges = false;
}

Character *Project::NewCharacter(const std::string &Name)
{
	Character *NewCharacter = new Character(*this, _NodeView, Name);
	_Characters.insert({ Name, NewCharacter });

	return NewCharacter;
}

bool Project::DeleteCharacter(Character *DeleteChar)
{
	auto &Res = _Characters.find(DeleteChar->GetName());
	if (Res != _Characters.end())
	{
		delete Res->second;
		_Characters.erase(Res);
		return true;
	}

	return false;
}

std::map<std::string, Character*> &Project::Characters()
{
	return _Characters;
}