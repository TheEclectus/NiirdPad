#pragma once

#include <deque>
#include <unordered_map>
#include <vector>

#include <SDL.h>

#include "QNodeView.h"

// GraphicsBlocks.h
class GraphicsBlock_NodeInputBox;
class GraphicsBlock_NodeOutputBox;
class GraphicsBlock_Node;

// Forward Declarations
class ConnectionManager;
class ConnectionTarget;
class NodeDialogue;
class NodeOption;

/*
	Validation function 

	One-to-One - Only ever has a single connection point.
	One-to-Many - Can have one or more named connection points.
	- Implement as map of strings.
	- Keys that are generated by the validation function that were also generated in the previous
	  call won't be cleared and deleted.
	One-to-Infinite - Can have any number of unnamed connection points.
	- Order shouldn't be assumed by the user; implement as deque.
*/

// Exists SOLELY to create and manage Connection objects. Everything that can act as the
// input for a connection must have a ConnectionManager.
class ConnectionManager
{
private:
public:
};

/*
	The representation of a connection (and its nature) between an Option and a Dialogue.
	Has no specialized type, because 
*/
class Connection
{
	friend class ConnectionManager;
private:
	std::string _key;	// <- the key that 'this' will be stored under
	size_t _index;		// <- the index of the infinite connection

	Connection(ConnectionManager *Src, ConnectionTarget *Tgt);
	~Connection();
public:
};

// Exists SOLELY for back-lookup of connected Options.
class ConnectionTarget
{

};

// TODO: In NodeDialogue and NodeOption, store the QScriptEditWindow's ::saveState() data and reset the state of the QSplitter (save this to the project file!)

// TODO: SetAll() must currently be called before _graphics is updated.
class NodeDialogue
{
private:
	// Non-owning
	GraphicsBlock_NodeInputBox *_graphics;

	// _script and _dialogue are multiline strings
	std::string _reference, _dialogue;	// Dialogue text has '\n' replaced with '<br>'s
	std::vector<std::string> _functionLines;
	//std::string _reference, _script, _dialogue;
public:
	NodeDialogue(GraphicsBlock_NodeInputBox *Graphics, const std::string &Reference, const std::vector<std::string> &FunctionLines = {}, const std::string &Dialogue = "");

	void SetAll(const std::vector<std::string> &FunctionLines, const std::string &Dialogue);
	void SetReference(const std::string &Reference);
	void SetFunctions(const std::vector<std::string> &FunctionLines);
	// Passing a string with newline characters is ill-formed
	void SetDialogue(const std::string &Dialogue);

	GraphicsBlock_NodeInputBox *Graphics();
	ConnectionTarget &ConnTarget();
};

// TODO: SetAll() must currently be called before _graphics is updated.
class NodeOption
{
private:
	// Non-owning
	GraphicsBlock_NodeOutputBox *_graphics;

	// Non-owning
	/*
		TODO: Connection types:
		- Direct (one-to-one)
		- Named (one-to-many, named)
	*/
	//NodeDialogue *_

	// All but _pointer are multiline strings
	//std::string _pointer, _visibilityScripts, _functions, _text;
	std::string _option;
	std::vector<std::string> _visibilityScriptLines, _functionLines;
public:
	NodeOption(GraphicsBlock_NodeOutputBox *Graphics, const std::vector<std::string> &VisibilityScripts = {}, const const std::vector<std::string> &Functions = {}, const std::string &Text = "");

	void SetAll(const std::vector<std::string> &VisibilityScriptLines, const std::vector<std::string> &FunctionLines, const std::string &Option);
	void SetVisibilityLines(const std::vector<std::string> &VisibilityLines);
	void SetFunctions(const std::vector<std::string> &FunctionLines);
	void SetOption(const std::string &Dialogue);

	GraphicsBlock_NodeOutputBox *Graphics();
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