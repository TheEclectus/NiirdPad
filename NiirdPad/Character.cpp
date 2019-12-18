#include "Character.h"

#include "DialogueFile.h"
#include "Project.h"
#include "QNodeView.h"

#include <rapidjson\document.h>	
#include <rapidjson\rapidjson.h>

Character::Character(Project &ParentProject, QNodeView &NodeView, const std::string &Name) :
	_parentProject(ParentProject),
	_nodeView(NodeView),
	_name(Name)
{

}

const std::string &Character::GetName() const
{
	return _name;
}

QNodeView &Character::GetNodeView() const
{
	return _nodeView;
}

DialogueFile *Character::NewDialogueFile(const std::string &Filename)
{
	auto Res = _dialogueFiles.find(Filename);
	if (Res != _dialogueFiles.end())
		return nullptr;

	DialogueFile *NewFile = new DialogueFile(*this, Filename);
	_dialogueFiles.insert({ Filename, NewFile });

	return NewFile;
}

std::map<std::string, DialogueFile*> &Character::DialogueFiles()
{
	return _dialogueFiles;
}

void Character::Save(rapidjson::Document &Doc, rapidjson::Value &Value) const
{
	rapidjson::Value Comment("", 0);
	Value.AddMember("comment", Comment, Doc.GetAllocator());

	rapidjson::Value Name(_name.c_str(), _name.length(), Doc.GetAllocator());
	Value.AddMember("name", Name, Doc.GetAllocator());

	rapidjson::Value DialogueFiles(rapidjson::kArrayType);
	for (auto &CurFileIter : _dialogueFiles)
	{
		rapidjson::Value CurFile(rapidjson::kObjectType);
		CurFileIter.second->Save(Doc, CurFile);

		DialogueFiles.PushBack(CurFile, Doc.GetAllocator());
	}
	Value.AddMember("files", DialogueFiles, Doc.GetAllocator());
}

//bool Character::SaveAs(const std::string &Path)
//{
//	rapidjson::Document Doc;
//	Doc.SetObject();
//
//	// TODO: Implement Character comments.
//	rapidjson::Value Comment("", 0);
//	Doc.AddMember("comment", Comment, Doc.GetAllocator());
//
//	rapidjson::Value Name(_name.c_str(), _name.length(), Doc.GetAllocator());
//	Doc.AddMember("name", Name, Doc.GetAllocator());
//
//	rapidjson::Value DialogueFiles(rapidjson::kArrayType);
//	for (auto &CurFileIter : _dialogueFiles)
//	{
//		rapidjson::Value CurFile(rapidjson::kObjectType);
//		CurFileIter.second->Save(Doc, CurFile);
//
//		DialogueFiles.PushBack(CurFile, Doc.GetAllocator());
//	}
//	Doc.AddMember("files", DialogueFiles, Doc.GetAllocator());
//
//	std::ofstream OutStream(Path);
//	rapidjson::OStreamWrapper OutStreamWrapper(OutStream);
//	rapidjson::Writer<rapidjson::OStreamWrapper> Writer(OutStreamWrapper);
//
//	return Doc.Accept(Writer);
//}