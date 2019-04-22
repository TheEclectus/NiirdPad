#pragma once

#include <vector>

#include <SDL.h>

#include "QNodeView.h"

// GraphicsBlocks.h
class GraphicsBlock_NodeInputBox;
class GraphicsBlock_NodeOutputBox;
class GraphicsBlock_Node;

class NodeDialogue
{
private:
	// Non-owning
	GraphicsBlock_NodeInputBox *_graphics;

	std::string _reference, _script, _dialogue;
public:
	NodeDialogue(SDL_Renderer *Renderer, const std::string &Reference, const std::string &Script = "", const std::string &Dialogue = "");

	void SetAll(const std::string &Reference, std::string &Script, const std::string &Dialogue);
	void SetReference(const std::string &Reference);
	void SetScriptLines(const std::string &Script);
	void SetDialogue(const std::string &Dialogue);

	GraphicsBlock_NodeInputBox *Graphics();
};

class Node
{
private:
	// Owning
	GraphicsBlock_Node *_graphics;
	std::vector<NodeDialogue*> _dialogues;

public:
	Node(QNodeView &NodeView);
	~Node();

	GraphicsBlock_Node &Graphics();

	NodeDialogue *AddDialogue();
	void RemoveDialogue(NodeDialogue *Dlg);
	const std::vector<const NodeDialogue *const> &Dialogues() const;
};