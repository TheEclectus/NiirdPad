#include "Node.h"

#include "GraphicsBlocks.h"


NodeDialogue::NodeDialogue(GraphicsBlock_NodeInputBox *Graphics, const std::string &Reference, const std::vector<std::string> &FunctionLines, const std::string &Dialogue) :
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

GraphicsBlock_NodeInputBox *NodeDialogue::Graphics()
{
	return _graphics;
}



NodeOption::NodeOption(GraphicsBlock_NodeOutputBox *Graphics, const std::vector<std::string> &VisibilityScripts, const const std::vector<std::string> &Functions, const std::string &Text) :
	_graphics(Graphics),
	_visibilityScriptLines(VisibilityScripts),
	_functionLines(Functions),
	_option(Text)
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

const SDL_Point &Node::Position()
{
	return _position;
}

NodeDialogue *Node::AddDialogue(const std::string &Reference)
{
	GraphicsBlock_NodeInputBox *NewInputBoxGraphics = _graphics->InputSection()->AddInputBox();
	NodeDialogue *NewDialogue = new NodeDialogue(NewInputBoxGraphics, "", {}, "");
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
	NodeOption *NewOption = new NodeOption(NewOutputBoxGraphics);

	_options.push_back(NewOption);

	return NewOption;
}

bool Node::FeatureAtPosition(SDL_Point MousePos, NodeDialogue **const Dlg, NodeOption **const Opt)
{
	//auto Header = _graphics->Header();
	auto Inputs = _graphics->InputSection();
	auto Outputs = _graphics->OutputSection();

	// Input section
	if (SDL_PointInRect(&MousePos, &Inputs->GetBounds()))
	{
		for (auto CurrentDialogue : _dialogues)
		{
			auto Dialogue = CurrentDialogue->Graphics()->GetBounds();

			if (SDL_PointInRect(&MousePos, &Dialogue))
			{
				*Dlg = CurrentDialogue;
				return true;
			}
		}
	}
	// Output section
	else if (SDL_PointInRect(&MousePos, &Outputs->GetBounds()))
	{

	}

	// TODO: implement header checking, and be able to return it as a parameter

	return false;
}