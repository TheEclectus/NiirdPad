#include "Project.h"

#include <experimental\filesystem>
#include <fstream>

#include <rapidjson\document.h>
#include <rapidjson\rapidjson.h>
#include <rapidjson\ostreamwrapper.h>
#include <rapidjson\writer.h>
#include <QFileDialog>

#include "Character.h"
#include "DialogueFile.h"
#include "NiirdPad.h"
#include "Node.h"
#include "RawProjectFile.h"

Project::Project(QNodeView &NodeView, const std::string &Path) :
	Project(NodeView)
{


	_bUnsavedChanges = false;
}

Project::Project(QNodeView &NodeView, const RawProjectFile &ImportedProject) :
	Project(NodeView)
{
	// TODO: Replace CurrentOffset with a proper layout algorithm.

	int CurrentOffset = 0;
	for (auto &CurrentCharacter : ImportedProject.Characters())
	{
		const std::string &CharacterName = CurrentCharacter.first;
		const auto &CharacterData = CurrentCharacter.second;

		Character *NewChar = NewCharacter(CharacterName);
		for (auto &CurrentFile : CharacterData.Files())
		{
			const std::string &FileName = CurrentFile.first;
			auto &FileData = CurrentFile.second;

			DialogueFile *NewFile = NewChar->NewDialogueFile(FileName);
			for (auto &CurrentNode : FileData.Nodes())
			{
				Node *NewNode = NewFile->NewNode();
				// HACK: Right here, see TODO above.
				NewNode->SetPosition({ CurrentOffset, 0 });

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
				}
			}
		}
	}
}

Project::Project(QNodeView &NodeView) :
	_NodeView(NodeView)
{

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

std::map<std::string, Character*> &Project::Characters()
{
	return _Characters;
}