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



Node::Node(QNodeView &NodeView) :
	_graphics(new GraphicsBlock_Node(NodeView.SDLRenderer(), NodeView.FontStore().GetFont(FontStore::Role::NodeComment)))
{
	
}

Node::~Node()
{

}

GraphicsBlock_Node &Node::Graphics()
{
	return *_graphics;
}

NodeDialogue *Node::AddDialogue()
{
	
}