#include "Node.h"

#include "GraphicsBlocks.h"


NodeDialogue::NodeDialogue(SDL_Renderer *Renderer, const std::string &Reference, const std::string &Script, const std::string &Dialogue)
{
	//_graphics = new GraphicsBlock_NodeInputBox(Renderer, )
}

Node::Node(QNodeView &NodeView) :
	_graphics(new GraphicsBlock_Node(NodeView.SDLRenderer(), NodeView.FontStore().GetFont(FontStore::Role::NodeComment)))
{

}