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

class DialogueFile
{
private:
	Character &_parentCharacter;
	std::string _filename;
	std::map<std::string, NodeDialogue*> _indices;	// non-owning NodeDialogue pointers
	std::vector<Node*> _nodes;
public:
	DialogueFile(Character &ParentCharacter, const std::string &Filename);

	const std::string &GetFilename() const;
	const std::map<std::string, NodeDialogue*> &GetIndices() const;
	const std::vector<Node*> &GetNodes() const;

	Node *NewNode(const SDL_Point &Position = { 0,0 });
};