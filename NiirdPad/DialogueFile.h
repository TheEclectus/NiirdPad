#pragma once

#include <map>
#include <string>
#include <vector>

#include <rapidjson\document.h>
#include <rapidjson\rapidjson.h>
#include <SDL.h>

#include "Camera.h"

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
	//QNodeViewCamera _Camera;
	//std::map<std::string, NodeDialogue*> _indices;	// non-owning NodeDialogue pointers
	std::vector<Node*> _nodes;
public:
	DialogueFile(Character &ParentCharacter, const std::string &Filename);

	Character &ParentCharacter();

	ReferenceDatabase &GetReferenceDatabase();
	const std::string &GetFilename() const;
	//QNodeViewCamera &GetCamera();
	//const std::map<std::string, NodeDialogue*> &GetIndices() const;
	const std::vector<Node*> &GetNodes() const;

	void Save(rapidjson::Document &Doc, rapidjson::Value &Value) const;

	Node *NewNode(const SDL_Point &Position = { 0,0 });
};