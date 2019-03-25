#include "GraphicsBlocks.h"

#include <algorithm>

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
	_Text = Text;

	Dirty();
	return *this;
}

GraphicsBlock_Text &GraphicsBlock_Text::SetText(const std::string &Text, const int MaxWidth)
{
	_MaxTextWidth = std::max(-1, MaxWidth);

	return SetText(Text);
}

void GraphicsBlock_Text::CalculateSize()
{
	// TODO: Store preferred alignment.
	if (_MaxTextWidth == -1)
	{
		_CalculatedBounds = FC_GetBounds(_Font, 0.f, 0.f, FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _Text.c_str());
	}
	else
	{
		_CalculatedText.clear();
		_CalculatedText.reserve(_Text.size() * 1.1f);
		int CharCount = FC_GetWrappedText(_Font, (char*)_CalculatedText.c_str(), _CalculatedText.capacity(), _MaxTextWidth, _Text.c_str());
		_CalculatedText.shrink_to_fit();

		_CalculatedBounds = FC_GetBounds(_Font, 0.f, 0.f, FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _Text.c_str());
	}

	// There shouldn't be any children in a text GraphicsBlock. 
	//AGraphicsBlock::CalculateSize();
}

void GraphicsBlock_Text::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	FC_Effect Effect = FC_MakeEffect(FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _FontColor);
	FC_DrawBoxEffect(_Font, SDLRenderer, _CalculatedBounds, Effect, _Text.c_str());
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