#include "Node.h"

#include <QFile>

#include "Character.h"
#include "DialogueFile.h"
#include "GraphicsBlocks.h"
#include "NiirdPad.h"
#include "ReferenceDatabase.h"

ConnectionInput::ConnectionInput(NubInput &Parent, const std::vector<ConnectionOutput*> &Connections) :
	_parent(Parent),
	_incomingConnections(Connections)
{

}

ConnectionInput::~ConnectionInput()
{
	for (auto CurConn : _incomingConnections)
	{
		CurConn->Disconnect();
	}
}

NubInput &ConnectionInput::Parent()
{
	return _parent;
}

const std::vector<ConnectionOutput*> &ConnectionInput::IncomingConnections()
{
	return _incomingConnections;
}

void ConnectionInput::Connect(ConnectionOutput *Destination)
{
	auto Res = std::find(_incomingConnections.begin(), _incomingConnections.end(), Destination);

	// There's already a connection.
	if (Res != _incomingConnections.end())
		return;

	_incomingConnections.push_back(Destination);
}

void ConnectionInput::Disconnect(ConnectionOutput *Destination)
{
	auto Res = std::find(_incomingConnections.begin(), _incomingConnections.end(), Destination);
	
	// There's no connection.
	if (Res == _incomingConnections.end())
		return;

	_incomingConnections.erase(Res);
}



ConnectionOutput::ConnectionOutput(NubOutput &Parent, const std::string &KeyName, ConnectionInput *Connection) :
	_parent(Parent),
	_keyName(KeyName),
	_connection(Connection)
{

}

ConnectionOutput::~ConnectionOutput()
{
	Disconnect();
}

NubOutput &ConnectionOutput::Parent()
{
	return _parent;
}

const std::string &ConnectionOutput::KeyName()
{
	return _keyName;
}

ConnectionInput *ConnectionOutput::Connection()
{
	return _connection;
}

void ConnectionOutput::Connect(ConnectionInput *Destination)
{
	// If it's already connected to the destination, save some steps.
	if (_connection == Destination)
		return;

	// Disconnect the original connection if it exists.
	if (_connection)
		this->Disconnect();

	// Add self to the list of connections on the ConnectionInput side.
	Destination->Connect(this);
	// Set the destination as the active connection 
	_connection = Destination;

	fmt::print("Connected {} to {}.\n", fmt::ptr(this), fmt::ptr(Destination));
}

void ConnectionOutput::Disconnect()
{
	// If there's no active connection, don't do anything.
	if (_connection == nullptr)
		return;

	fmt::print("Disconnected {} from {}", fmt::ptr(this), fmt::ptr(_connection));

	// Disconnect from the ConnectionInput side.
	_connection->Disconnect(this);
	// Set the connection to nullptr.
	_connection = nullptr;
}



// Saving for now, for posterity and/or recycling.
//void ConnectionPointInput::SetKeys(const std::vector<std::string> &Keys)
//{
//	std::vector<ConnectionInput*> NewConnections;
//
//	for (const std::string &CurrentKey : Keys)
//	{
//		// If the key has a matching existing name, sustain the connection.
//		auto FoundKey = std::find_if(_connections.begin(), _connections.end(), [CurrentKey](ConnectionInput* Conn)
//		{
//			if (Conn->KeyName() == CurrentKey)
//			{
//				return true;
//			}
//
//			return false;
//		});
//
//		if (FoundKey != _connections.end()) // found
//		{
//			NewConnections.push_back(*FoundKey);
//			_connections.erase(FoundKey);
//		}
//		else
//		{
//			NewConnections.push_back(new ConnectionInput(*this, CurrentKey, nullptr));
//		}
//	}
//
//	for (auto RemainingKey : _connections)
//	{
//		ConnectionInput *Conn = RemainingKey;
//		delete Conn;
//	}
//	_connections.clear();
//	_connections = NewConnections;
//}



SDL_Texture *ANub::_NubTextureDefault = nullptr;
SDL_Texture *ANub::_NubTextureHighlighted = nullptr;
SDL_Rect ANub::_NubTextureSize = { 0, 0, 0, 0 };

void ANub::LoadTextures(SDL_Renderer *Renderer)
{
	if (_NubTextureDefault == nullptr)
	{
		QFile NubDefault(":/NiirdPad/Resources/nub_full_default2.bmp");
		if (NubDefault.open(QIODevice::OpenModeFlag::ReadOnly))
		{
			auto Bytes = NubDefault.readAll();

			//SDL_Surface *Temp = SDL_CreateRGBSurfaceFrom(Bytes.data(), 9, 15, 24, 9 * 3, 0x0000FF, 0x00FF00, 0xFF0000, 0x000000);
			SDL_Surface *Temp = SDL_LoadBMP_RW(SDL_RWFromConstMem(Bytes.data(), Bytes.size()), 1);
			SDL_SetColorKey(Temp, 1, 0x00FF00);
			_NubTextureSize = { 0, 0, Temp->w, Temp->h };
			_NubTextureDefault = SDL_CreateTextureFromSurface(Renderer, Temp);
			SDL_FreeSurface(Temp);
		}
	}

	if (_NubTextureHighlighted == nullptr)
	{
		QFile NubHighlighted(":/NiirdPad/Resources/nub_full_highlight2.bmp");
		if (NubHighlighted.open(QIODevice::OpenModeFlag::ReadOnly))
		{
			auto Bytes = NubHighlighted.readAll();

			//SDL_Surface *Temp = SDL_CreateRGBSurfaceFrom(Bytes.data(), 9, 15, 24, 9 * 3, 0x0000FF, 0x00FF00, 0xFF0000, 0x000000);
			SDL_Surface *Temp = SDL_LoadBMP_RW(SDL_RWFromConstMem(Bytes.data(), Bytes.size()), 1);
			SDL_SetColorKey(Temp, 1, 0x00FF00);
			//_NubTextureSize = { 0, 0, Temp->w, Temp->h };
			_NubTextureHighlighted = SDL_CreateTextureFromSurface(Renderer, Temp);
			SDL_FreeSurface(Temp);
		}
	}
}

SDL_Texture *ANub::TextureDefault()
{
	return NubOutput::_NubTextureDefault;
}

SDL_Texture *ANub::TextureHighlighted()
{
	return NubOutput::_NubTextureHighlighted;
}

const SDL_Rect &ANub::TextureSize()
{
	return NubOutput::_NubTextureSize;
}



NubInput::NubInput(NodeDialogue &Parent) :
	_parent(Parent),
	_connection(*this, {})
{

}

NodeDialogue &NubInput::Parent()
{
	return _parent;
}

const ANub::NubType NubInput::GetNubType()
{
	return ANub::NubType::Input;
}

ConnectionInput &NubInput::Connection()
{
	return _connection;
}



NubOutput::NubOutput(NodeOption &Parent) :
	_parent(Parent)
{
	
}

NubOutput::~NubOutput()
{
	for (auto CurConn : _connections)
	{
		delete CurConn;
	}
}

NodeOption &NubOutput::Parent()
{
	return _parent;
}

const ANub::NubType NubOutput::GetNubType()
{
	return NubType::Output;
}

void NubOutput::SetConnectionKeys(const std::vector<std::string> &Keys)
{
	std::vector<ConnectionOutput*> NewConnections;
	std::unordered_map<std::string, ConnectionOutput*> OldConns;
	for (auto &CurOldConn : _connections)
	{
		if (CurOldConn->Connection() != nullptr)	// Live connections only; unused ones are pruned.
			OldConns.insert({ CurOldConn->KeyName(), CurOldConn });
	}

	// Compare the new connections against the old ones. If there's a match, then maintain the old connection pointer.
	for (auto &NewKey : Keys)
	{
		auto Res = OldConns.find(NewKey);
		if (Res != OldConns.end())
		{
			NewConnections.push_back((*Res).second);	// Add it (should be added sequentially?)
			OldConns.erase(Res);						// Remove it from the copied list of old connections
		}
		else // It's a new key
		{
			ConnectionOutput *NewConn = new ConnectionOutput(*this, NewKey, nullptr);

			NewConnections.push_back(NewConn);
		}
	}

	// Disconnect and delete the remaining old connections
	for (auto &CurOldConn : OldConns)
	{
		ConnectionOutput *OldConnPtr = CurOldConn.second;
		OldConnPtr->Disconnect();
		delete OldConnPtr;
	}

	_connections = NewConnections;
}

std::vector<ConnectionOutput*> &NubOutput::Connections()
{
	return _connections;
}



NodeDialogue::NodeDialogue(Node &ParentNode, GraphicsBlock_NodeInputBox *Graphics, const std::string &Reference, const std::vector<std::string> &FunctionLines, const std::string &Dialogue) :
	_parentNode(ParentNode),
	_graphics(Graphics),
	_reference(Reference),
	_dialogue(Dialogue),
	_functionLines(FunctionLines),
	_nub(*this)
{
	//_graphics = new GraphicsBlock_NodeInputBox(Renderer, )
}

NodeDialogue::~NodeDialogue()
{
	
}

Node &NodeDialogue::Parent()
{
	return _parentNode;
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

	auto &Engine = _parentNode.ParentFile().ParentCharacter().GetNodeView().GetNiirdPad()->ScriptEngine();
	bool bErrorFound = false;	// TODO: Currently not implemented. Consider changing the outline or fill color of the GraphicsBox.
	
	std::string FunctionLinesString = "";
	for (std::string Line : FunctionLines)
	{
		std::string FuncError = "";
		std::vector<std::string> Keys;
		if (!Engine.bScriptIsValid(Line, Keys, FuncError))
		{
			bErrorFound = true;
		}
		// LASTTIME: you were looking at ScriptEngine to make sure erroneous scripts go the "__default__" key as well as non-IM functions.

		if (FunctionLinesString.length() == 0)
			FunctionLinesString += Line;
		else
			FunctionLinesString += "\n" + Line;
	}

	_graphics->SetScript(FunctionLinesString);
}

void NodeDialogue::SetDialogue(const std::string &Dialogue)
{
	std::string FormattedDialogue = Dialogue;
	size_t Pos = FormattedDialogue.find('\n');
	while (Pos != std::string::npos)
	{
		FormattedDialogue.replace(Pos, 1, "<br>");
		Pos = FormattedDialogue.find('\n');
	}

	_dialogue = FormattedDialogue;
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

QByteArray &NodeDialogue::WindowState()
{
	return _windowState;
}

NubInput &NodeDialogue::Nub()
{
	return _nub;
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

Node &NodeOption::Parent()
{
	return _parentNode;
}

void NodeOption::SetAll(const std::vector<std::string> &VisibilityScriptLines, const std::vector<std::string> &FunctionLines, const std::string &Option)
{
	SetVisibilityLines(VisibilityScriptLines);
	SetFunctions(FunctionLines);
	SetOption(Option);
}

void NodeOption::SetVisibilityLines(const std::vector<std::string> &VisibilityLines)
{
	_visibilityScriptLines = VisibilityLines;

	// It's time to smoke crack
	// That's what we're smoking here
	// We're smoking crack
	auto &Engine = _parentNode.ParentFile().ParentCharacter().GetNodeView().GetNiirdPad()->ScriptEngine();
	bool bErrorFound = false;	// TODO: Currently not implemented. Consider changing the outline or fill color of the GraphicsBox.

	std::string VisLinesString = "";
	for (std::string Line : VisibilityLines)
	{
		// Checking for erroneous vis scripts here
		std::string VisError = "";
		if (!Engine.bVisConditionIsValid(Line, VisError))
		{
			bErrorFound = true;
		}

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

	auto &Engine = _parentNode.ParentFile().ParentCharacter().GetNodeView().GetNiirdPad()->ScriptEngine();
	bool bErrorFound = false;	// TODO: Currently not implemented. Consider changing the outline or fill color of the GraphicsBox.

	std::string FunctionLinesString = "";
	bool bHasIndexModifyingFunction = false;
	std::vector<std::string> PendingKeys;

	for (std::string Line : FunctionLines)
	{
		std::string FuncError = "";
		std::vector<std::string> Keys;
		if (!Engine.bScriptIsValid(Line, Keys, FuncError))
		{
			bErrorFound = true;
		}

		if (Keys.size() > 0)
		{
			if (!bHasIndexModifyingFunction)
			{
				bHasIndexModifyingFunction = true;
				PendingKeys = Keys;
			}
			else
			{
				bErrorFound = true;
			}
		}

		if (FunctionLinesString.length() == 0)
			FunctionLinesString += Line;
		else
			FunctionLinesString += "\n" + Line;
	}

	if (bErrorFound || !bHasIndexModifyingFunction)
	{
		PendingKeys = { "__default__" };	// Should be the only key
	}

	_nub.SetConnectionKeys(PendingKeys);

	// TODO: change SetScript to SetFunction or something. It's confusing as-is.
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

QByteArray &NodeOption::WindowState()
{
	return _windowState;
}

NubOutput &NodeOption::Nub()
{
	return _nub;
}

GraphicsBlock_NodeOutputBox *NodeOption::Graphics()
{
	return _graphics;
}



Node::Node(DialogueFile &Parent, QNodeView &NodeView, SDL_Point Position) :
	_parentFile(Parent),
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

DialogueFile &Node::ParentFile()
{
	return _parentFile;
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
	_comment = Comment;
	_graphics->Header()->SetText(Comment);
}

const std::string &Node::GetComment() const
{
	return _comment;
}

NodeDialogue *Node::AddDialogue(const std::string &Reference)
{
	auto &ReferenceDbase = _parentFile.GetReferenceDatabase();
	if (ReferenceDbase.Find(Reference) == nullptr)
	{
		GraphicsBlock_NodeInputBox *NewInputBoxGraphics = _graphics->InputSection()->AddInputBox();
		NodeDialogue *NewDialogue = new NodeDialogue(*this, NewInputBoxGraphics, "", {}, "");
		ReferenceDbase.New(Reference, NewDialogue);
		NewDialogue->SetReference(Reference);

		_dialogues.push_back(NewDialogue);

		return NewDialogue;
	}
	else
	{
		return nullptr;
	}
}

void Node::RemoveDialogue(NodeDialogue *Dlg)
{
	auto &ReferenceDbase = _parentFile.GetReferenceDatabase();

	auto Res = std::find(_dialogues.begin(), _dialogues.end(), Dlg);
	if (Res != _dialogues.end())
	{
		ReferenceDbase.Delete(Dlg->GetReference());
		_graphics->InputSection()->RemoveInputBox(Dlg->Graphics());
		delete Dlg;
		_dialogues.erase(Res);
	}
}

const std::vector<NodeDialogue*> &Node::Dialogues() const
{
	return _dialogues;
}

NodeOption *Node::AddOption()
{
	GraphicsBlock_NodeOutputBox *NewOutputBoxGraphics = _graphics->OutputSection()->AddOutputBox();
	NodeOption *NewOption = new NodeOption(*this, NewOutputBoxGraphics);

	_options.push_back(NewOption);

	return NewOption;
}

void Node::RemoveOption(NodeOption *Opt)
{
	auto Res = std::find(_options.begin(), _options.end(), Opt);
	if (Res != _options.end())
	{
		_graphics->OutputSection()->RemoveOutputBox(Opt->Graphics());
		delete Opt;
		_options.erase(Res);
	}
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
				break;
				//return true;
			}
		}
	}
	// Output section
	else if (SDL_PointInRect(&MousePos, &Outputs->GetBounds()))
	{
		bInOutputSection = true;

		for (auto CurrentOption : _options)
		{
			auto Option = CurrentOption->Graphics()->GetBounds();
			auto OptOffset = CurrentOption->Graphics()->GetTotalOffset();

			Option.x = OptOffset.x;
			Option.y = OptOffset.y;

			if (SDL_PointInRect(&MousePos, &Option))
			{
				*Opt = CurrentOption;
				break;
				//return true;
			}
		}
	}

	// TODO: implement header checking, and be able to return it as a parameter

	//return false;
}