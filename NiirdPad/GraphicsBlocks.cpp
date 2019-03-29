#include "GraphicsBlocks.h"

#include <algorithm>

#pragma region AGraphicsBlock

AGraphicsBlock::AGraphicsBlock(SDL_Renderer *AssociatedRenderer, const SDL_Rect &MaximumSize, const SDL_Rect &MinimumSize) :
	_Renderer(AssociatedRenderer),
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

void AGraphicsBlock::SetPosition(SDL_Point Pos)
{
	_CalculatedBounds.x = Pos.x;
	_CalculatedBounds.y = Pos.y;
}

const SDL_Rect &AGraphicsBlock::GetBounds() const
{
	return _CalculatedBounds;
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

#pragma region GraphicsBlock_Text

GraphicsBlock_Text::GraphicsBlock_Text(SDL_Renderer *AssociatedRenderer, FC_Font *Font) :
	AGraphicsBlock(AssociatedRenderer),
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
		size_t BufferSize = _Text.size() * 1.1f;
		char *Buffer = new char[BufferSize];

		int CharCount = FC_GetWrappedText(_Font, Buffer, BufferSize, _MaxTextWidth, _Text.c_str());
		_CalculatedText = Buffer;

		_CalculatedBounds = FC_GetBounds(_Font, 0.f, 0.f, FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _CalculatedText.c_str());
	}

	// There shouldn't be any children in a text GraphicsBlock. 
	//AGraphicsBlock::CalculateSize();
}

void GraphicsBlock_Text::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect DrawPos = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };

	FC_Effect Effect = FC_MakeEffect(FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _FontColor);
	auto res = FC_DrawBoxEffect(_Font, SDLRenderer, DrawPos, Effect, _CalculatedText.c_str());
	
	AGraphicsBlock::Render(SDLRenderer, Position);
}

#pragma endregion

GraphicsBlock_NodeHeader::GraphicsBlock_NodeHeader(SDL_Renderer *AssociatedRenderer, FC_Font *Font) :
	AGraphicsBlock(AssociatedRenderer),
	_Font(Font)
{
	_Label = new GraphicsBlock_Text(AssociatedRenderer, _Font);
	_Label->SetText("This is just test text. Texticles. 8=====D", 100);
	AddChild(_Label);
}

void GraphicsBlock_NodeHeader::CalculateSize()
{
	//_Label->CalculateSize();
	SDL_Rect Size = _Label->GetBounds();

	// Add a padding of 10 pixels on every side.
	_Label->SetPosition({ 10, 10 });

	Size.w += 20;
	Size.h += 20;

	_CalculatedBounds = Size;
}

void GraphicsBlock_NodeHeader::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect DrawPos = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	SDL_SetRenderDrawColor(SDLRenderer, 80, 80, 80, 255);
	SDL_RenderFillRect(SDLRenderer, &DrawPos);

	AGraphicsBlock::Render(SDLRenderer, Position);
}


GraphicsBlock_Node::GraphicsBlock_Node(SDL_Renderer *AssociatedRenderer, FC_Font *HeaderFont) :
	AGraphicsBlock(AssociatedRenderer)
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