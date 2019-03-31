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

void AGraphicsBlock::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	/*if (_MinimumSize.w && _CalculatedBounds.w < _MinimumSize.w)
		_CalculatedBounds.w = _MinimumSize.w;
	if (_MinimumSize.h && _CalculatedBounds.h < _MinimumSize.h)
		_CalculatedBounds.h = _MinimumSize.h;

	if (_MaximumSize.w && _CalculatedBounds.w < _MaximumSize.w)
		_CalculatedBounds.w = _MaximumSize.w;
	if (_MaximumSize.h && _CalculatedBounds.h < _MaximumSize.h)
		_CalculatedBounds.h = _MaximumSize.h;*/

	_SizeHint = { 0, 0, std::max(-1, MaxWidthHint), std::max(-1, MaxHeightHint) };
}

#pragma endregion

#pragma region GraphicsBlock_Text

GraphicsBlock_Text::GraphicsBlock_Text(SDL_Renderer *AssociatedRenderer, FC_Font *Font, const SDL_Color &Color) :
	AGraphicsBlock(AssociatedRenderer),
	_Font(Font),
	_FontColor(Color)
{

}

GraphicsBlock_Text &GraphicsBlock_Text::SetText(const std::string &Text)
{
	_Text = Text;

	Dirty();
	return *this;
}

void GraphicsBlock_Text::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	AGraphicsBlock::CalculateSize(MaxWidthHint, MaxHeightHint);

	// TODO: Store preferred alignment.
	if (_SizeHint.w == -1)
	{
		_CalculatedBounds = FC_GetBounds(_Font, 0.f, 0.f, FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _Text.c_str());
	}
	else
	{
		size_t BufferSize = _Text.size() * 1.1f;
		char *Buffer = new char[BufferSize];

		int CharCount = FC_GetWrappedText(_Font, Buffer, BufferSize,_SizeHint.w, _Text.c_str());
		_CalculatedText = Buffer;

		_CalculatedBounds = FC_GetBounds(_Font, 0.f, 0.f, FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _CalculatedText.c_str());

		if(_SizeHint.h != -1)
			_CalculatedBounds.h = std::min(_CalculatedBounds.h, _SizeHint.h);
	}

	// There shouldn't be any children in a text GraphicsBlock. 
	//AGraphicsBlock::CalculateSize();
}

void GraphicsBlock_Text::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect DrawPos = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };

	FC_Effect Effect = FC_MakeEffect(FC_AlignEnum::FC_ALIGN_LEFT, FC_MakeScale(1.f, 1.f), _FontColor);
	if(_SizeHint.w == -1)
		auto res = FC_DrawBoxEffect(_Font, SDLRenderer, DrawPos, Effect, _Text.c_str());
	else
		auto res = FC_DrawBoxEffect(_Font, SDLRenderer, DrawPos, Effect, _CalculatedText.c_str());
	
	AGraphicsBlock::Render(SDLRenderer, Position);
}

#pragma endregion

#pragma region GraphicsBlock_NodeHeader

GraphicsBlock_NodeHeader::GraphicsBlock_NodeHeader(SDL_Renderer *AssociatedRenderer, FC_Font *Font) :
	AGraphicsBlock(AssociatedRenderer),
	_Font(Font)
{
	_Label = new GraphicsBlock_Text(AssociatedRenderer, _Font, { 255, 255, 255, 255 });
	//_Label->SetText("sus", 200);
	AddChild(_Label);

	//Dirty();
}

void GraphicsBlock_NodeHeader::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	AGraphicsBlock::CalculateSize(MaxWidthHint, MaxHeightHint);

	// The maximum sizes of the header minus the padding.
	const int MaxLabelWidthSansPadding = (_SizeHint.w == -1 ? DEFAULT_WIDTH : _SizeHint.w) - (PADDING_LEFT + PADDING_RIGHT);
	const int MaxLabelHeightSansPadding = (_SizeHint.h == -1 ? DEFAULT_HEIGHT : _SizeHint.h) - (PADDING_TOP + PADDING_BOTTOM);

	_Label->CalculateSize(MaxLabelWidthSansPadding, MaxLabelHeightSansPadding);
	SDL_Rect Size = _Label->GetBounds();

	// Add a padding to position
	_Label->SetPosition({ PADDING_LEFT, PADDING_TOP });
	Size.w += PADDING_LEFT + PADDING_RIGHT;
	Size.h += PADDING_TOP + PADDING_BOTTOM;

	Size.w = std::max(Size.w, DEFAULT_WIDTH);
	Size.h = std::max(Size.h, DEFAULT_HEIGHT);

	_CalculatedBounds = Size;
}

void GraphicsBlock_NodeHeader::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect DrawPos = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	//SDL_SetRenderDrawColor(SDLRenderer, 80, 80, 80, 255);
	SDL_SetRenderDrawColor(SDLRenderer, 65, 65, 65, 255);
	SDL_RenderFillRect(SDLRenderer, &DrawPos);

	AGraphicsBlock::Render(SDLRenderer, Position);
}

void GraphicsBlock_NodeHeader::SetText(const std::string &Text)
{
	_Label->SetText(Text);
}

#pragma endregion

#pragma region GraphicsBlock_Node

GraphicsBlock_Node::GraphicsBlock_Node(SDL_Renderer *AssociatedRenderer, FC_Font *HeaderFont) :
	AGraphicsBlock(AssociatedRenderer)
{
	_Header = new GraphicsBlock_NodeHeader(AssociatedRenderer, HeaderFont);
	_Header->SetText("Test text.  dzfgzdfg Texticles. 8===========D");
	AddChild(_Header);
}

void GraphicsBlock_Node::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	_Header->CalculateSize();
	SDL_Rect HeaderSize = _Header->GetBounds();

	_CalculatedBounds = HeaderSize;
}

void GraphicsBlock_Node::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	/*SDL_Rect RenderDest = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	SDL_SetRenderDrawColor(SDLRenderer, 80, 80, 80, 255);
	SDL_RenderFillRect(SDLRenderer, &RenderDest);*/

	AGraphicsBlock::Render(SDLRenderer, Position);
}

#pragma endregion