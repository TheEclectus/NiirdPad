#include "DialogueFile.h"

#include "Character.h"
#include "Node.h"
#include "QNodeView.h"

DialogueFile::DialogueFile(Character &ParentCharacter, std::string &Filename) :
	_parentCharacter(ParentCharacter),
	_filename(Filename)
{

}

const std::string &DialogueFile::GetFilename() const
{
	return _filename;
}

const std::map<std::string, NodeDialogue*> &DialogueFile::GetIndices() const
{
	return _indices;
}

Node *DialogueFile::NewNode(const SDL_Point &Position)
{
	auto &NodeView = _parentCharacter.GetNodeView();

	Node *NewNode = new Node(NodeView, Position);
	_nodes.push_back(NewNode);

	return NewNode;
}