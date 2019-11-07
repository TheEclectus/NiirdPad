#include "Character.h"

#include "DialogueFile.h"
#include "Project.h"
#include "QNodeView.h"

Character::Character(Project &ParentProject, QNodeView &NodeView/*, const std::string &Name*/) :
	_parentProject(ParentProject),
	_nodeView(NodeView)
	//_name(Name)
{

}

//const std::string &Character::GetName() const
//{
//	return _name;
//}

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