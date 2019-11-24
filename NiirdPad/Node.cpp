#include "Node.h"

#include <QFile>

#include "GraphicsBlocks.h"

ConnectionInput::ConnectionInput(ConnectionPointInput &Parent, const std::string &KeyName, ConnectionOutput *Connection) :
	_parent(Parent),
	_keyName(KeyName),
	_connection(Connection)
{

}

ConnectionPointInput &ConnectionInput::Parent()
{
	return _parent;
}

const std::string &ConnectionInput::KeyName()
{
	return _keyName;
}

ConnectionOutput *ConnectionInput::Connection()
{
	return _connection;
}

void ConnectionInput::SetConnection(ConnectionOutput *NewConnection)
{
	_connection = NewConnection;
}



ConnectionPointInput::ConnectionPointInput(NodeDialogue &Parent) :
	_parent(Parent)
{

}

NodeDialogue &ConnectionPointInput::Parent()
{
	return _parent;
}

std::vector<ConnectionInput*> &ConnectionPointInput::Connections()
{
	return _connections;
}

void ConnectionPointInput::SetKeys(const std::vector<std::string> &Keys)
{
	std::vector<ConnectionInput*> NewConnections;

	for (const std::string &CurrentKey : Keys)
	{
		// If the key has a matching existing name, sustain the connection.
		auto FoundKey = std::find_if(_connections.begin(), _connections.end(), [CurrentKey](ConnectionInput* Conn)
		{
			if (Conn->KeyName() == CurrentKey)
			{
				return true;
			}

			return false;
		});

		if (FoundKey != _connections.end()) // found
		{
			NewConnections.push_back(*FoundKey);
			_connections.erase(FoundKey);
		}
		else
		{
			NewConnections.push_back(new ConnectionInput(*this, CurrentKey, nullptr));
		}
	}

	for (auto RemainingKey : _connections)
	{
		ConnectionInput *Conn = RemainingKey;
		delete Conn;
	}
	_connections.clear();
	_connections = NewConnections;
}



SDL_Texture *NubOutput::_NubTexture = nullptr;
SDL_Rect NubOutput::_NubTextureSize = { 0, 0, 0, 0 };

void NubOutput::LoadTexture(SDL_Renderer *Renderer)
{
	if (_NubTexture == nullptr)
	{
		QFile Nub(":/NiirdPad/Resources/nub_full_default2.bmp");
		if (Nub.open(QIODevice::OpenModeFlag::ReadOnly))
		{
			auto Bytes = Nub.readAll();

			//SDL_Surface *Temp = SDL_CreateRGBSurfaceFrom(Bytes.data(), 9, 15, 24, 9 * 3, 0x0000FF, 0x00FF00, 0xFF0000, 0x000000);
			SDL_Surface *Temp = SDL_LoadBMP_RW(SDL_RWFromConstMem(Bytes.data(), Bytes.size()), 1);
			SDL_SetColorKey(Temp, 1, 0x00FF00);
			_NubTextureSize = { 0, 0, Temp->w, Temp->h };
			_NubTexture = SDL_CreateTextureFromSurface(Renderer, Temp);
			SDL_FreeSurface(Temp);
		}
	}
}

SDL_Texture *NubOutput::Texture()
{
	return NubOutput::_NubTexture;
}

NubOutput::NubOutput(NodeOption &Parent) :
	_parent(Parent)
{
	
}



NodeDialogue::NodeDialogue(Node &ParentNode, GraphicsBlock_NodeInputBox *Graphics, const std::string &Reference, const std::vector<std::string> &FunctionLines, const std::string &Dialogue) :
	_parentNode(ParentNode),
	_graphics(Graphics),
	_reference(Reference),
	_dialogue(Dialogue),
	_functionLines(FunctionLines)
{
	//_graphics = new GraphicsBlock_NodeInputBox(Renderer, )
}

void NodeDialogue::SetAll(const std::vector<std::string> &FunctionLines, const std::string &Dialogue)
{
	SetFunctions(FunctionLines);
	SetDialogue(Dialogue);
}

void NodeDialogue::SetReference(const std::string &Reference)
{
	_reference = Reference;
	_graphics->SetIndex(Reference);
}

void NodeDialogue::SetFunctions(const std::vector<std::string> &FunctionLines)
{
	_functionLines = FunctionLines;
	std::string FunctionLinesString = "";
	for (std::string Line : FunctionLines)
	{
		if (FunctionLinesString.length() == 0)
			FunctionLinesString += Line;
		else
			FunctionLinesString += "\n" + Line;
	}

	_graphics->SetScript(FunctionLinesString);
}

void NodeDialogue::SetDialogue(const std::string &Dialogue)
{
	_dialogue = Dialogue;
	_graphics->SetDialogue(Dialogue);
}

const std::string &NodeDialogue::GetReference() const
{
	return _reference;
}

const std::string &NodeDialogue::GetDialogue() const
{
	return _dialogue;
}

const std::vector<std::string> &NodeDialogue::GetFunctionLines() const
{
	return _functionLines;
}

GraphicsBlock_NodeInputBox *NodeDialogue::Graphics()
{
	return _graphics;
}



NodeOption::NodeOption(Node &ParentNode, GraphicsBlock_NodeOutputBox *Graphics, const std::vector<std::string> &VisibilityScripts, const const std::vector<std::string> &Functions, const std::string &Text) :
	_parentNode(ParentNode),
	_graphics(Graphics),
	_visibilityScriptLines(VisibilityScripts),
	_functionLines(Functions),
	_option(Text),
	_nub(*this)
{
	
}

void NodeOption::SetAll(const std::vector<std::string> &VisibilityScriptLines, const std::vector<std::string> &FunctionLines, const std::string &Option)
{
	SetVisibilityLines(VisibilityScriptLines);
	SetFunctions(FunctionLines);
	SetOption(Option);
}

void NodeOption::SetVisibilityLines(const std::vector<std::string> &VisibilityLines)
{
	_functionLines = VisibilityLines;
	std::string VisLinesString = "";
	for (std::string Line : VisibilityLines)
	{
		if (VisLinesString.length() == 0)
			VisLinesString += Line;
		else
			VisLinesString += "\n" + Line;
	}
	_graphics->SetVisibility(VisLinesString);
}

void NodeOption::SetFunctions(const std::vector<std::string> &FunctionLines)
{
	_functionLines = FunctionLines;
	std::string FunctionLinesString = "";
	for (std::string Line : FunctionLines)
	{
		if (FunctionLinesString.length() == 0)
			FunctionLinesString += Line;
		else
			FunctionLinesString += "\n" + Line;
	}

	_graphics->SetScript(FunctionLinesString);
}

void NodeOption::SetOption(const std::string &Dialogue)
{
	_option = Dialogue;
	_graphics->SetOption(Dialogue);
}

const std::string &NodeOption::GetOption() const
{
	return _option;
}

const std::vector<std::string> &NodeOption::GetFunctionLines() const
{
	return _functionLines;
}

const std::vector<std::string> &NodeOption::GetVisibilityScriptLines() const
{
	return _visibilityScriptLines;
}

NubOutput &NodeOption::Nub()
{
	return _nub;
}

GraphicsBlock_NodeOutputBox *NodeOption::Graphics()
{
	return _graphics;
}



Node::Node(QNodeView &NodeView, SDL_Point Position) :
	_graphics(new GraphicsBlock_Node(NodeView.SDLRenderer(), NodeView.FontStore().GetFont(FontStore::Role::NodeComment))),
	_position(Position)
{
	
}

Node::~Node()
{

}

GraphicsBlock_Node &Node::Graphics()
{
	return *_graphics;
}

void Node::SetPosition(const SDL_Point &Position)
{
	_position = Position;
}

const SDL_Point &Node::Position()
{
	return _position;
}

void Node::SetComment(const std::string &Comment)
{
	_graphics->Header()->SetText(Comment);
}

NodeDialogue *Node::AddDialogue(const std::string &Reference)
{
	GraphicsBlock_NodeInputBox *NewInputBoxGraphics = _graphics->InputSection()->AddInputBox();
	NodeDialogue *NewDialogue = new NodeDialogue(*this, NewInputBoxGraphics, "", {}, "");
	NewDialogue->SetReference(Reference);

	_dialogues.push_back(NewDialogue);

	return NewDialogue;
}

void Node::RemoveDialogue(NodeDialogue *Dlg)
{
	auto Res = std::find(_dialogues.begin(), _dialogues.end(), Dlg);
	if (Res != _dialogues.end())
	{
		_dialogues.erase(Res);
	}

	_graphics->InputSection()->RemoveInputBox(Dlg->Graphics());
}

NodeOption *Node::AddOption()
{
	GraphicsBlock_NodeOutputBox *NewOutputBoxGraphics = _graphics->OutputSection()->AddOutputBox();
	NodeOption *NewOption = new NodeOption(*this, NewOutputBoxGraphics);

	_options.push_back(NewOption);

	return NewOption;
}

const std::vector<NodeOption*> &Node::Options() const
{
	return _options;
}

void Node::FeatureAtPosition(SDL_Point MousePos, bool &bInHeader, bool &bInInputSection, bool &bInOutputSection, NodeDialogue **const Dlg, NodeOption **const Opt)
{
	auto Header = _graphics->Header();
	auto Inputs = _graphics->InputSection();
	auto Outputs = _graphics->OutputSection();

	// Input section
	if (SDL_PointInRect(&MousePos, &Header->GetBounds()))
	{
		bInHeader = true;
	}
	else if (SDL_PointInRect(&MousePos, &Inputs->GetBounds()))
	{
		bInInputSection = true;

		for (auto CurrentDialogue : _dialogues)
		{
			auto Dialogue = CurrentDialogue->Graphics()->GetBounds();
			auto DlgOffset = CurrentDialogue->Graphics()->GetTotalOffset();

			Dialogue.x = DlgOffset.x;
			Dialogue.y = DlgOffset.y;

			if (SDL_PointInRect(&MousePos, &Dialogue))
			{
				*Dlg = CurrentDialogue;
				//return true;
			}
		}
	}
	// Output section
	else if (SDL_PointInRect(&MousePos, &Outputs->GetBounds()))
	{
		bInOutputSection = true;
	}

	// TODO: implement header checking, and be able to return it as a parameter

	//return false;
}