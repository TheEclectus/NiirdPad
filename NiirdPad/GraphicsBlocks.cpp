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
		if(!_bIsConstructing)
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
		_CalculatedText = _Text;
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
	auto res = FC_DrawBoxEffect(_Font, SDLRenderer, DrawPos, Effect, _CalculatedText.c_str());
	//SDL_SetRenderDrawColor(SDLRenderer, 0, 255, 0, 255);
	//SDL_RenderDrawRect(SDLRenderer, &res);

	//SDL_SetRenderDrawColor(SDLRenderer, 255, 0, 0, 255);
	//SDL_RenderDrawRect(SDLRenderer, &_CalculatedBounds);
	
	/*if(_SizeHint.w == -1)
		auto res = FC_DrawBoxEffect(_Font, SDLRenderer, DrawPos, Effect, _Text.c_str());
	else
		auto res = FC_DrawBoxEffect(_Font, SDLRenderer, DrawPos, Effect, _CalculatedText.c_str());*/
	
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

#pragma region GraphicsBlock_NodeInputBox

GraphicsBlock_NodeInputBox::GraphicsBlock_NodeInputBox(SDL_Renderer *AssociatedRenderer, FC_Font *TextFont, FC_Font *ScriptFont, const SDL_Color &TextColor, const SDL_Color &ScriptColor/*, const SDL_Color &VisibilityScriptColor*/) :
	AGraphicsBlock(AssociatedRenderer),
	_TextFont(TextFont),
	_TextColor(TextColor),
	_ScriptFont(ScriptFont),
	_ScriptColor(ScriptColor)
{
	_IndexLabel = new GraphicsBlock_Text(AssociatedRenderer, TextFont, TextColor);
	_ScriptLabel = new GraphicsBlock_Text(AssociatedRenderer, ScriptFont, ScriptColor);
	_DialogueLabel = new GraphicsBlock_Text(AssociatedRenderer, TextFont, TextColor);

	AddChild(_IndexLabel);
	AddChild(_ScriptLabel);
	AddChild(_DialogueLabel);
}

void GraphicsBlock_NodeInputBox::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	AGraphicsBlock::CalculateSize(MaxWidthHint, MaxHeightHint);

	// The maximum width is determined by _IndexLabel or _ScriptLabel, whichever is larger.
	//SDL_Rect Size = { 0, 0, 0, 0 };
	_IndexLabel->CalculateSize();
	_ScriptLabel->CalculateSize();
	const int WidestElement = std::max(_IndexLabel->GetBounds().w, _ScriptLabel->GetBounds().w);
	const int TextWidth = std::max(WidestElement, DEFAULT_WIDTH) - (PADDING_LEFT + PADDING_RIGHT);
	_DialogueLabel->CalculateSize(TextWidth);

	SDL_Rect Size = _IndexLabel->GetBounds();
	Size.h += PADDING_TOP;
	_IndexLabel->SetPosition({ PADDING_LEFT, PADDING_TOP });
	//Size.h += _IndexLabel->GetBounds().h;

	Size.h += TEXT_SPACING;
	//_ScriptLabel->SetPosition({ PADDING_LEFT, PADDING_TOP + _IndexLabel->GetBounds().y + _IndexLabel->GetBounds().h });
	_ScriptLabel->SetPosition({ PADDING_LEFT, Size.h });
	Size.h += _ScriptLabel->GetBounds().h;

	Size.h += TEXT_SPACING;
	//_DialogueLabel->SetPosition({ PADDING_LEFT, PADDING_TOP + _ScriptLabel->GetBounds().y + _ScriptLabel->GetBounds().h });
	_DialogueLabel->SetPosition({ PADDING_LEFT, Size.h });
	Size.h += _DialogueLabel->GetBounds().h;

	Size.h += PADDING_BOTTOM;
	Size.h = std::max(Size.h, DEFAULT_HEIGHT);

	Size.w = std::max({ _IndexLabel->GetBounds().w, _ScriptLabel->GetBounds().w, _DialogueLabel->GetBounds().w });
	Size.w += PADDING_LEFT + PADDING_RIGHT;
	Size.w = std::max(Size.w, DEFAULT_WIDTH);

	_CalculatedBounds = Size;
}

void GraphicsBlock_NodeInputBox::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect DrawPos = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	SDL_SetRenderDrawColor(SDLRenderer, 80, 80, 80, 255);
	//SDL_SetRenderDrawColor(SDLRenderer, 65, 65, 65, 255);
	SDL_RenderFillRect(SDLRenderer, &DrawPos);
	SDL_SetRenderDrawColor(SDLRenderer, 110, 110, 110, 255);
	SDL_RenderDrawRect(SDLRenderer, &DrawPos);

	AGraphicsBlock::Render(SDLRenderer, Position);
}

void GraphicsBlock_NodeInputBox::SetIndex(const std::string &Text)
{
	_IndexLabel->SetText(Text);
}

void GraphicsBlock_NodeInputBox::SetScript(const std::string &Text)
{
	_ScriptLabel->SetText(Text);
}

void GraphicsBlock_NodeInputBox::SetDialogue(const std::string &Text)
{
	_DialogueLabel->SetText(Text);
}

#pragma endregion

#pragma region GraphicsBlock_NodeInputBoxSection

GraphicsBlock_NodeInputBoxSection::GraphicsBlock_NodeInputBoxSection(SDL_Renderer *AssociatedRenderer, FC_Font *TextFont, FC_Font *ScriptFont, const SDL_Color &TextColor, const SDL_Color &ScriptColor) :
	AGraphicsBlock(AssociatedRenderer),
	_TextFont(TextFont),
	_TextColor(TextColor),
	_ScriptFont(ScriptFont),
	_ScriptColor(ScriptColor)
{

}

GraphicsBlock_NodeInputBox *GraphicsBlock_NodeInputBoxSection::AddInputBox()
{
	auto NewBox = new GraphicsBlock_NodeInputBox(this->_Renderer, this->_TextFont, this->_ScriptFont);

	AddChild(NewBox);
	_InputBoxes.push_back(NewBox);

	return NewBox;
}

void GraphicsBlock_NodeInputBoxSection::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	SDL_Rect Size = { 0, 0, 0, 0 };

	for (int i = 0; i < _InputBoxes.size(); i++)
	{
		auto CurBox = _InputBoxes[i];

		bool bAddSpacing = (_InputBoxes.size() > 1) && (i < (_InputBoxes.size() - 1));
		const int VertSpacing = (bAddSpacing ? SPACING : 0);

		CurBox->CalculateSize();
		CurBox->SetPosition({ PADDING_LEFT, PADDING_TOP + Size.h + VertSpacing });
		Size.w = std::max(Size.w, CurBox->GetBounds().w);
		Size.h += CurBox->GetBounds().h + VertSpacing;
	}

	Size.w += PADDING_LEFT + PADDING_RIGHT;
	Size.h += PADDING_TOP + PADDING_BOTTOM;

	Size.w = std::max(Size.w, DEFAULT_WIDTH);
	Size.h = std::max(Size.h, DEFAULT_HEIGHT);

	_CalculatedBounds = Size;
}

void GraphicsBlock_NodeInputBoxSection::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect DrawPos = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	SDL_SetRenderDrawColor(SDLRenderer, 80, 80, 80, 255);
	SDL_RenderFillRect(SDLRenderer, &DrawPos);

	AGraphicsBlock::Render(SDLRenderer, Position);
}

#pragma endregion

#pragma region GraphicsBlock_NodeOutputBox

GraphicsBlock_NodeOutputBox::GraphicsBlock_NodeOutputBox(SDL_Renderer *AssociatedRenderer, FC_Font *TextFont, FC_Font *ScriptFont, const SDL_Color &TextColor, const SDL_Color &ScriptColor, const SDL_Color &VisibilityScriptColor) :
	AGraphicsBlock(AssociatedRenderer),
	_TextFont(TextFont),
	_TextColor(TextColor),
	_ScriptFont(ScriptFont),
	_ScriptColor(ScriptColor)
{
	_VisibilityLabel = new GraphicsBlock_Text(AssociatedRenderer, TextFont, VisibilityScriptColor);
	_ScriptLabel = new GraphicsBlock_Text(AssociatedRenderer, ScriptFont, ScriptColor);
	_OptionLabel = new GraphicsBlock_Text(AssociatedRenderer, TextFont, TextColor);

	AddChild(_VisibilityLabel);
	AddChild(_ScriptLabel);
	AddChild(_OptionLabel);
}

void GraphicsBlock_NodeOutputBox::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	AGraphicsBlock::CalculateSize(MaxWidthHint, MaxHeightHint);

	// The maximum width is determined by _IndexLabel or _ScriptLabel, whichever is larger.
	//SDL_Rect Size = { 0, 0, 0, 0 };
	_VisibilityLabel->CalculateSize();
	_ScriptLabel->CalculateSize();
	const int WidestElement = std::max(_VisibilityLabel->GetBounds().w, _ScriptLabel->GetBounds().w);
	const int TextWidth = std::max(WidestElement, DEFAULT_WIDTH) - (PADDING_LEFT + PADDING_RIGHT);
	_OptionLabel->CalculateSize(TextWidth);

	SDL_Rect Size = _VisibilityLabel->GetBounds();
	Size.h += PADDING_TOP;
	_VisibilityLabel->SetPosition({ PADDING_LEFT, PADDING_TOP });
	//Size.h += _VisibilityLabel->GetBounds().h;

	Size.h += TEXT_SPACING;
	_ScriptLabel->SetPosition({ PADDING_LEFT, Size.h });
	Size.h += _ScriptLabel->GetBounds().h;

	Size.h += TEXT_SPACING;
	_OptionLabel->SetPosition({ PADDING_LEFT, Size.h });
	Size.h += _OptionLabel->GetBounds().h;

	Size.h += PADDING_BOTTOM;
	Size.h = std::max(Size.h, DEFAULT_HEIGHT);

	Size.w = std::max({ _VisibilityLabel->GetBounds().w, _ScriptLabel->GetBounds().w, _OptionLabel->GetBounds().w });
	Size.w += PADDING_LEFT + PADDING_RIGHT;
	Size.w = std::max(Size.w, DEFAULT_WIDTH);

	_CalculatedBounds = Size;
}

void GraphicsBlock_NodeOutputBox::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect DrawPos = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	SDL_SetRenderDrawColor(SDLRenderer, 110, 110, 110, 255);
	SDL_RenderFillRect(SDLRenderer, &DrawPos);
	SDL_SetRenderDrawColor(SDLRenderer, 80, 80, 80, 255);
	SDL_RenderDrawRect(SDLRenderer, &DrawPos);

	AGraphicsBlock::Render(SDLRenderer, Position);
}

void GraphicsBlock_NodeOutputBox::SetVisibility(const std::string &Text)
{
	_VisibilityLabel->SetText(Text);
}

void GraphicsBlock_NodeOutputBox::SetScript(const std::string &Text)
{
	_ScriptLabel->SetText(Text);
}

void GraphicsBlock_NodeOutputBox::SetOption(const std::string &Text)
{
	_OptionLabel->SetText(Text);
}


#pragma endregion

#pragma region GraphicsBlock_NodeOutputBoxSection

GraphicsBlock_NodeOutputBoxSection::GraphicsBlock_NodeOutputBoxSection(SDL_Renderer *AssociatedRenderer, FC_Font *TextFont, FC_Font *ScriptFont, FC_Font *VisFont, const SDL_Color &TextColor, const SDL_Color &ScriptColor, const SDL_Color &VisColor) :
	AGraphicsBlock(AssociatedRenderer),
	_TextFont(TextFont),
	_TextColor(TextColor),
	_ScriptFont(ScriptFont),
	_ScriptColor(ScriptColor)
{

}

GraphicsBlock_NodeOutputBox *GraphicsBlock_NodeOutputBoxSection::AddOutputBox()
{
	auto NewBox = new GraphicsBlock_NodeOutputBox(this->_Renderer, this->_TextFont, this->_ScriptFont);

	AddChild(NewBox);
	_OutputBoxes.push_back(NewBox);

	return NewBox;
}

void GraphicsBlock_NodeOutputBoxSection::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	SDL_Rect Size = { 0, 0, 0, 0 };

	for (int i = 0; i < _OutputBoxes.size(); i++)
	{
		auto CurBox = _OutputBoxes[i];

		bool bAddSpacing = (_OutputBoxes.size() > 1) && (i < (_OutputBoxes.size() - 1));
		const int VertSpacing = (bAddSpacing ? SPACING : 0);

		CurBox->CalculateSize();

		SDL_Point DestinationPosition = {
			this->GetBounds().w - (CurBox->GetBounds().w + PADDING_RIGHT),
			PADDING_TOP + Size.h + VertSpacing
		};

		CurBox->SetPosition(DestinationPosition);
		Size.w = std::max(Size.w, CurBox->GetBounds().w);
		Size.h += CurBox->GetBounds().h + VertSpacing;
	}

	Size.w += PADDING_LEFT + PADDING_RIGHT;
	Size.h += PADDING_TOP + PADDING_BOTTOM;

	Size.w = std::max(Size.w, DEFAULT_WIDTH);
	Size.h = std::max(Size.h, DEFAULT_HEIGHT);

	_CalculatedBounds = Size;
}

void GraphicsBlock_NodeOutputBoxSection::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	SDL_Rect DrawPos = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	SDL_SetRenderDrawColor(SDLRenderer, 110, 110, 110, 110);
	SDL_RenderFillRect(SDLRenderer, &DrawPos);

	AGraphicsBlock::Render(SDLRenderer, Position);
}

#pragma endregion


#pragma region GraphicsBlock_Node

GraphicsBlock_Node::GraphicsBlock_Node(SDL_Renderer *AssociatedRenderer, FC_Font *HeaderFont) :
	AGraphicsBlock(AssociatedRenderer)
{
	_bIsConstructing = true;

	_Header = new GraphicsBlock_NodeHeader(AssociatedRenderer, HeaderFont);
	_Header->SetText("Test text.  dzfgzdfg Texticles. 8===========D");
	AddChild(_Header);

	_Inputs = new GraphicsBlock_NodeInputBoxSection(AssociatedRenderer, HeaderFont, HeaderFont);
	AddChild(_Inputs);
	
	auto NewInputBox = _Inputs->AddInputBox();
	NewInputBox->SetIndex("start");
	NewInputBox->SetScript("no script, just testing.");
	NewInputBox->SetDialogue("There never was a story of more woe, than that of Juliet, and her Romeo.");

	_Outputs = new GraphicsBlock_NodeOutputBoxSection(AssociatedRenderer, HeaderFont, HeaderFont, HeaderFont);
	AddChild(_Outputs);

	auto NewOutputBox = _Outputs->AddOutputBox();
	NewOutputBox->SetVisibility("//showif.has_flag.eated_the_cheese");
	NewOutputBox->SetScript("dummy test script");
	NewOutputBox->SetOption("I want to eated the other cheese owo");

	_bIsConstructing = false;
	CalculateSize();
}

void GraphicsBlock_Node::CalculateSize(int MaxWidthHint, int MaxHeightHint)
{
	SDL_Rect Size = { 0, 0, 0, 0 };

	_Header->CalculateSize();
	SDL_Rect HeaderSize = _Header->GetBounds();
	Size.h += HeaderSize.h;

	_Inputs->CalculateSize();
	_Inputs->SetPosition({ 0, Size.h });
	SDL_Rect InputSectionSize = _Inputs->GetBounds();
	Size.h += InputSectionSize.h;

	_Outputs->CalculateSize();
	_Outputs->SetPosition({ 0, Size.h });
	SDL_Rect OutputSectionSize = _Outputs->GetBounds();
	Size.h += InputSectionSize.h;

	Size.w = std::max({ HeaderSize.w, InputSectionSize.w, OutputSectionSize.w });

	_CalculatedBounds = Size;
}

void GraphicsBlock_Node::Render(SDL_Renderer *SDLRenderer, SDL_Point Position)
{
	/*SDL_Rect RenderDest = { Position.x, Position.y, _CalculatedBounds.w, _CalculatedBounds.h };
	SDL_SetRenderDrawColor(SDLRenderer, 80, 80, 80, 255);
	SDL_RenderFillRect(SDLRenderer, &RenderDest);*/

	AGraphicsBlock::Render(SDLRenderer, Position);
}

GraphicsBlock_NodeInputBoxSection *GraphicsBlock_Node::InputSection()
{
	return _Inputs;
}

GraphicsBlock_NodeOutputBoxSection *GraphicsBlock_Node::OutputSection()
{
	return _Outputs;
}

#pragma endregion