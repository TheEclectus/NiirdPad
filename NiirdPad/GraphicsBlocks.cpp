#include "GraphicsBlocks.h"

#pragma region AGraphicsBlock

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

void AGraphicsBlock::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	if (_ChildBlocks.size() > 0)
	{
		for (auto Child : _ChildBlocks)
		{
			SDL_Point ChildRenderPos = { Position.x + Child->_CalculatedBounds.x, Position.y + Child->_CalculatedBounds.y };
			Child->Render(SDLRenderer, ChildRenderPos);
		}
	}
}

void AGraphicsBlock::AddChild(AGraphicsBlock *Child)
{
	Child->_ParentBlock = this;
	_ChildBlocks.push_back(Child);
}

void AGraphicsBlock::Dirty()
{
	if (_ParentBlock)
	{
		_ParentBlock->Dirty();
	}
	else
	{
		CalculateSize();
	}
}

#pragma endregion

GraphicsBlock_Text::GraphicsBlock_Text(FC_Font *Font) :
	_Font(Font)
{

}

GraphicsBlock_Text &GraphicsBlock_Text::SetText(const std::string &Text)
{


	if (_ParentBlock)
		_ParentBlock->CalculateSize();

	return *this;
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
	SDL_SetRenderDrawColor(SDLRenderer, 80, 80, 80, 255);
	SDL_RenderFillRect(SDLRenderer, &RenderDest);

	AGraphicsBlock::Render(SDLRenderer, Position);
}