#pragma once

#include <map>
#include <string>
#include <vector>

#include <SDL.h>

// Character.h
class Character;

// Node.h
class Node;
class NodeDialogue;

// ReferenceDatabase.h
class ReferenceDatabase;

// TODO: Store camera information
class DialogueFile
{
private:
	Character &_parentCharacter;
	ReferenceDatabase *_referenceDatabase;
	std::string _filename;
	std::map<std::string, NodeDialogue*> _indices;	// non-owning NodeDialogue pointers
	std::vector<Node*> _nodes;
public:
	DialogueFile(Character &ParentCharacter, const std::string &Filename);

	ReferenceDatabase &GetReferenceDatabase();
	const std::string &GetFilename() const;
	const std::map<std::string, NodeDialogue*> &GetIndices() const;
	const std::vector<Node*> &GetNodes() const;

	Node *NewNode(const SDL_Point &Position = { 0,0 });
};