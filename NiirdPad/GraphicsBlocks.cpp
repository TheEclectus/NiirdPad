#include "GraphicsBlocks.h"

AGraphicsBlock::AGraphicsBlock(const SDL_Rect &MaximumSize, const SDL_Rect &MinimumSize) :
	_MaximumSize(MaximumSize),
	_MinimumSize(MinimumSize)
{

}

AGraphicsBlock::~AGraphicsBlock()
{
	if (_ChildBlocks.size() > 0u)
	{
		for (auto Child : _ChildBlocks)
		{
			delete Child;
		}
		_ChildBlocks.clear();
		
	}
}

void AGraphicsBlock::AddChild(AGraphicsBlock *Child)
{
	Child->_ParentBlock = this;
	_ChildBlocks.push_back(Child);
}

GraphicsBlock_Node::GraphicsBlock_Node() :
	AGraphicsBlock()
{

}

void GraphicsBlock_Node::CalculateSize()
{
	_CalculatedBounds = { 0, 0, 100, 100 };
}

void GraphicsBlock_Node::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect RenderDest = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	SDL_SetRenderDrawColor(SDLRenderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(SDLRenderer, &RenderDest);
}