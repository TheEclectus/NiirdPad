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

// DialogueFile.h
class DialogueFile;

// Forward Declarations
class ConnectionInput;
class ConnectionPointInput;

class ConnectionOutput;
class ConnectionPointOutput;

class ANub;
class NubOutput;

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

// Represents a single connection between a named key and a target.
// Owned by a ConnectionPoint, which is basically a map of AConnections.
class ConnectionInput
{
private:
	ConnectionPointInput &_parent;
	std::string _keyName;
	ConnectionOutput *_connection = nullptr;
public:
	ConnectionInput(ConnectionPointInput &Parent, const std::string &KeyName, ConnectionOutput *Connection = nullptr);
	ConnectionPointInput &Parent();
	const std::string &KeyName();
	ConnectionOutput *Connection();
	void SetConnection(ConnectionOutput *NewConnection);
};

class ConnectionPointInput
{
private:
	NodeDialogue &_parent;
	std::vector<ConnectionInput*> _connections;
public:
	ConnectionPointInput(NodeDialogue &Parent);
	NodeDialogue &Parent();
	std::vector<ConnectionInput*> &Connections();
	void SetKeys(const std::vector<std::string> &Keys);
};

class ConnectionOutput
{
private:
	NodeOption &_parent;
	std::string _keyName;
	ConnectionInput *_connection = nullptr;
public:
	ConnectionOutput(const std::string &KeyName, ConnectionInput *Connection = nullptr);
	NodeOption &Parent();
	const std::string &KeyName();
	ConnectionInput *Connection();
};

class NubOutput
{
private:
	static SDL_Texture *_NubTexture;
	static SDL_Rect _NubTextureSize;

	NodeOption &_parent;
	std::vector<ConnectionOutput*> _connections;

	// "Positionless"? On mouse move events, just check in a radius around the NubPoint()
	//SDL_Point _position = { 0, 0 };
public:
	static void LoadTexture(SDL_Renderer *Renderer);
	static SDL_Texture *Texture();

	NubOutput(NodeOption &Parent);
};

// TODO: In NodeDialogue and NodeOption, store the QScriptEditWindow's ::saveState() data and reset the state of the QSplitter (save this to the project file!)
// TODO: SetAll() must currently be called before _graphics is updated.
class NodeDialogue
{
private:
	Node &_parentNode;
	// Non-owning
	GraphicsBlock_NodeInputBox *_graphics;

	// _script and _dialogue are multiline strings
	std::string _reference, _dialogue;	// Dialogue text has '\n' replaced with '<br>'s
	std::vector<std::string> _functionLines;
	//std::string _reference, _script, _dialogue;
public:
	NodeDialogue(Node &ParentNode, GraphicsBlock_NodeInputBox *Graphics, const std::string &Reference, const std::vector<std::string> &FunctionLines = {}, const std::string &Dialogue = "");

	void SetAll(const std::vector<std::string> &FunctionLines, const std::string &Dialogue);
	void SetReference(const std::string &Reference);
	void SetFunctions(const std::vector<std::string> &FunctionLines);
	// Passing a string with newline characters is ill-formed
	void SetDialogue(const std::string &Dialogue);

	const std::string &GetReference() const;
	const std::string &GetDialogue() const;
	const std::vector<std::string> &GetFunctionLines() const;

	GraphicsBlock_NodeInputBox *Graphics();
};

// TODO: SetAll() must currently be called before _graphics is updated.
class NodeOption
{
private:
	Node &_parentNode;
	// Non-owning
	GraphicsBlock_NodeOutputBox *_graphics;

	// All but _pointer are multiline strings
	//std::string _pointer, _visibilityScripts, _functions, _text;
	std::string _option;
	std::vector<std::string> _visibilityScriptLines, _functionLines;

	NubOutput _nub;
public:
	NodeOption(Node &ParentNode, GraphicsBlock_NodeOutputBox *Graphics, const std::vector<std::string> &VisibilityScripts = {}, const const std::vector<std::string> &Functions = {}, const std::string &Text = "");

	void SetAll(const std::vector<std::string> &VisibilityScriptLines, const std::vector<std::string> &FunctionLines, const std::string &Option);
	void SetVisibilityLines(const std::vector<std::string> &VisibilityLines);
	void SetFunctions(const std::vector<std::string> &FunctionLines);
	void SetOption(const std::string &Dialogue);

	const std::string &GetOption() const;
	const std::vector<std::string> &GetFunctionLines() const;
	const std::vector<std::string> &GetVisibilityScriptLines() const;

	NubOutput &Nub();

	GraphicsBlock_NodeOutputBox *Graphics();
};

class Node
{
private:
	//DialogueFile &_parentFile;
	// Owning
	GraphicsBlock_Node *_graphics;
	std::vector<NodeDialogue*> _dialogues;
	std::vector<NodeOption*> _options;

	SDL_Point _position;

public:
	Node(QNodeView &NodeView, SDL_Point Position = { 0,0 });
	~Node();

	GraphicsBlock_Node &Graphics();

	void SetPosition(const SDL_Point &Position);
	const SDL_Point &Position();

	void SetComment(const std::string &Comment);

	NodeDialogue *AddDialogue(const std::string &Reference);
	void RemoveDialogue(NodeDialogue *Dlg);
	const std::vector<const NodeDialogue*> &Dialogues() const;

	NodeOption *AddOption();
	const std::vector<NodeOption*> &Options() const;

	// MousePos must be adjusted for position. [0,0] is top-left
	void FeatureAtPosition(SDL_Point MousePos, bool &bInHeader, bool &bInInputSection, bool &bInOutputSection, NodeDialogue **const Dlg, NodeOption **const Opt);
};