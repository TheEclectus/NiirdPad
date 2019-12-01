#include "DialogueFile.h"

#include "Character.h"
#include "Node.h"
#include "QNodeView.h"
#include "ReferenceDatabase.h"

DialogueFile::DialogueFile(Character &ParentCharacter, const std::string &Filename) :
	_parentCharacter(ParentCharacter),
	_referenceDatabase(new ReferenceDatabase()),
	_filename(Filename)
{

}

Character &DialogueFile::ParentCharacter()
{
	return _parentCharacter;
}

ReferenceDatabase &DialogueFile::GetReferenceDatabase()
{
	return *_referenceDatabase;
}

const std::string &DialogueFile::GetFilename() const
{
	return _filename;
}

const std::map<std::string, NodeDialogue*> &DialogueFile::GetIndices() const
{
	return _indices;
}

const std::vector<Node*> &DialogueFile::GetNodes() const
{
	return _nodes;
}

Node *DialogueFile::NewNode(const SDL_Point &Position)
{
	auto &NodeView = _parentCharacter.GetNodeView();

	Node *NewNode = new Node(*this, NodeView, Position);
	_nodes.push_back(NewNode);

	return NewNode;
}