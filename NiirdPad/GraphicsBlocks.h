#pragma once

#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_FontCache.h"

/// <summary>
/// A graphical building block class. Has faculties for defining the bounding box
/// of a graphical object based on its own criteria, and that of blocks within it.
/// 
/// General design principles:
/// - Only include data that is absolutely necessary to draw the GraphicsBlock.
/// </summary>
/* 
	TODO: Consider adding signals and signal handlers that relay a message and data to
	child Blocks. Thsi would be useful for when fonts or colors are changed.
*/ 
class AGraphicsBlock
{
public:
	enum class Justification
	{
		Left,
		Center,
		Right
	};
protected:
	AGraphicsBlock *_ParentBlock = nullptr;
	std::vector<AGraphicsBlock*> _ChildBlocks;
	SDL_Renderer *_Renderer = nullptr;
	bool _bIsConstructing = false;

	/// <summary>
	/// Defines the maximum size of the block. -1 in either width or height field means no limit.
	/// </summary>
	SDL_Rect _MaximumSize;

	/// <summary>
	/// Defines the minimum size of a block.
	/// </summary>
	SDL_Rect _MinimumSize;

	/// <summary>
	/// The most-recent size hints passed to CalculateSize().
	/// </summary>
	SDL_Rect _SizeHint;

	/// <summary>
	/// The calculated dimensions and offset from the parent (uppermost block is always 0,0 
	/// position).
	/// </summary>
	SDL_Rect _CalculatedBounds = { 0, 0, 0, 0 };

public:
	AGraphicsBlock(SDL_Renderer *AssociatedRenderer, const SDL_Rect &MaximumSize = { 0, 0, -1, -1 }, const SDL_Rect &MinimumSize = { 0, 0, 0, 0 });
	virtual ~AGraphicsBlock();

	/// <summary>
	/// Set the position of the Block.
	/// </summary>
	/// <param name="Pos"></param>
	void SetPosition(SDL_Point Pos);

	const SDL_Rect &GetBounds() const;

	/// <summary>
	/// Adds an AGraphicsBlock as a child, ensuring it will be properly assigned this as its parent and cleaned up on destruction.
	/// </summary>
	/// <param name="Child">The AGraphicsBlock to add as a child.</param>
	void AddChild(AGraphicsBlock *Child);

	/// <summary>
	/// Removes a child AGraphicsBlock. Will do nothing if the pointer is not a child. Does
	/// NOT delete the child.
	/// </summary>
	/// <param name="Child">The AGraphicsBlock to remove.</param>
	void RemoveChild(AGraphicsBlock *Child);

	/// <summary>
	/// Signals that the GraphicsBlock must have CalculateSize() called again. Will call
	/// Dirty() of _ParentBlock unless it is nullptr, at which point CalculateSize() will
	/// be called. Top-most block will not have CalculateSize() called until _bIsConstructing
	/// is false, which is set by calling CalculateSize() explicity.
	/// </summary>
	void Dirty();

	/// <summary>
	/// Renders the AGraphicsBlock to an SDL_Renderer at a specified point. Calls
	/// Render() on all child AGraphicsBlocks as well with proper offsets.
	/// </summary>
	/// <param name="SDLRenderer"></param>
	/// <param name="Position"></param>
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position);

	/// <summary>
	/// Calculates the bounds of the block based on the return values of its children and
	/// minimum/maximum sizes. Should never call Dirty(). If either size hint is not -1,
	/// then the final calculated size must fit the bounds they define.
	/// </summary>
	virtual void CalculateSize(int MaxWidthHint = -1, int MaxHeightHint = -1);
};

class GraphicsBlock_Text : public AGraphicsBlock
{
protected:
	FC_Font *_Font = nullptr;
	SDL_Color _FontColor;
	std::string _Text = "";
	std::string _CalculatedText = "";
	/// <summary>
	/// The maximum width of the rendered text in pixels. -1 means no limit.
	/// </summary>
	//int _MaxTextWidth = -1;
public:
	GraphicsBlock_Text(SDL_Renderer *AssociatedRenderer, FC_Font *Font, const SDL_Color &Color = { 0, 0, 0, 255 });
	
	GraphicsBlock_Text &SetText(const std::string &Text);
	const std::string &GetText() const;

	GraphicsBlock_Text &SetMaxWidth(const int MaxWidth);
	const int GetMaxWidth() const;

	virtual void CalculateSize(int MaxWidthHint = -1, int MaxHeightHint = -1) override;
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) override;
};

class GraphicsBlock_NodeHeader : public AGraphicsBlock
{
protected:
	static const int	PADDING_LEFT = 10,
						PADDING_RIGHT = 10,
						PADDING_TOP	= 5,
						PADDING_BOTTOM = 5;

	static const int	DEFAULT_WIDTH = 300,
						DEFAULT_HEIGHT = 25;

	FC_Font *_Font = nullptr;
	GraphicsBlock_Text *_Label = nullptr;

public:
	GraphicsBlock_NodeHeader(SDL_Renderer *AssociatedRenderer, FC_Font *Font);

	virtual void CalculateSize(int MaxWidthHint = -1, int MaxHeightHint = -1) override;
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) override;

	void SetText(const std::string &Text);
};

class GraphicsBlock_NodeInputBox : public AGraphicsBlock
{
protected:
	static const int	PADDING_LEFT = 5,
						PADDING_RIGHT = 5,
						PADDING_TOP	= 5,
						PADDING_BOTTOM = 5,
						TEXT_SPACING = 3;

	static const int	DEFAULT_WIDTH = 200,
						DEFAULT_HEIGHT = 35;

	FC_Font *_TextFont, *_ScriptFont;
	SDL_Color _TextColor, _ScriptColor;// , _VisibilityScriptColor;

	// Labels contain text formatted solely for display purposes.
	GraphicsBlock_Text *_IndexLabel;
	GraphicsBlock_Text *_ScriptLabel;
	GraphicsBlock_Text *_DialogueLabel;

public:
	GraphicsBlock_NodeInputBox(SDL_Renderer *AssociatedRenderer, FC_Font *TextFont, FC_Font *ScriptFont, const SDL_Color &TextColor = { 255, 255, 255, 255 }, const SDL_Color &ScriptColor = { 235, 195, 85, 255 }/*, const SDL_Color &VisibilityScriptColor = { 173, 216, 230, 255 }*/);

	virtual void CalculateSize(int MaxWidthHint = -1, int MaxHeightHint = -1) override;
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) override;

	void SetIndex(const std::string &Text);
	void SetScript(const std::string &Text);
	void SetDialogue(const std::string &Text);
};

class GraphicsBlock_NodeInputBoxSection : public AGraphicsBlock
{
protected:
	static const int	PADDING_LEFT = 5,
						PADDING_RIGHT = 5,
						PADDING_TOP	= 5,
						PADDING_BOTTOM = 5,
						SPACING = 5;

	static const int	DEFAULT_WIDTH = 300,
						DEFAULT_HEIGHT = 45;

	FC_Font *_TextFont, *_ScriptFont;
	SDL_Color _TextColor, _ScriptColor;

	std::vector<GraphicsBlock_NodeInputBox*> _InputBoxes;

public:
	GraphicsBlock_NodeInputBoxSection(SDL_Renderer *AssociatedRenderer, FC_Font *TextFont, FC_Font *ScriptFont, const SDL_Color &TextColor = { 255, 255, 255, 255 }, const SDL_Color &ScriptColor = { 173, 216, 230, 255 });
	GraphicsBlock_NodeInputBox *AddInputBox();
	void RemoveInputBox(GraphicsBlock_NodeInputBox *Target);

	virtual void CalculateSize(int MaxWidthHint = -1, int MaxHeightHint = -1) override;
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) override;
};

class GraphicsBlock_NodeOutputBox : public AGraphicsBlock
{
protected:
	static const int	PADDING_LEFT = 5,
						PADDING_RIGHT = 5,
						PADDING_TOP = 5,
						PADDING_BOTTOM = 5,
						TEXT_SPACING = 3;

	static const int	DEFAULT_WIDTH = 200,
						DEFAULT_HEIGHT = 35;

	FC_Font *_TextFont, *_ScriptFont;
	SDL_Color _TextColor, _ScriptColor;// , _VisibilityScriptColor;

	// Labels contain text formatted solely for display purposes.
	GraphicsBlock_Text *_VisibilityLabel;
	GraphicsBlock_Text *_OptionLabel;
	GraphicsBlock_Text *_ScriptLabel;

public:
	GraphicsBlock_NodeOutputBox(SDL_Renderer *AssociatedRenderer, FC_Font *TextFont, FC_Font *ScriptFont, const SDL_Color &TextColor = { 255, 255, 255, 255 }, const SDL_Color &ScriptColor = { 173, 216, 230, 255 }, const SDL_Color &VisibilityScriptColor = { 235, 195, 85, 255 } );

	virtual void CalculateSize(int MaxWidthHint = -1, int MaxHeightHint = -1) override;
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) override;

	void SetVisibility(const std::string &Text);
	void SetOption(const std::string &Text);
	void SetScript(const std::string &Text);
	
};

class GraphicsBlock_NodeOutputBoxSection : public AGraphicsBlock
{
protected:
	static const int	PADDING_LEFT = 5,
						PADDING_RIGHT = 5,
						PADDING_TOP = 5,
						PADDING_BOTTOM = 5,
						SPACING = 5;

	static const int	DEFAULT_WIDTH = 300,
						DEFAULT_HEIGHT = 45;

	FC_Font *_TextFont, *_ScriptFont, *_VisFont;
	SDL_Color _TextColor, _ScriptColor, *_VisColor;

	std::vector<GraphicsBlock_NodeOutputBox*> _OutputBoxes;

public:
	GraphicsBlock_NodeOutputBoxSection(SDL_Renderer *AssociatedRenderer, FC_Font *TextFont, FC_Font *ScriptFont, FC_Font *VisFont, const SDL_Color &TextColor = { 255, 255, 255, 255 }, const SDL_Color &ScriptColor = { 173, 216, 230, 255 }, const SDL_Color &VisColor = { 235, 195, 85, 255 });
	GraphicsBlock_NodeOutputBox *AddOutputBox();

	virtual void CalculateSize(int MaxWidthHint = -1, int MaxHeightHint = -1) override;
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) override;
};

class GraphicsBlock_Node : public AGraphicsBlock
{
protected:
	GraphicsBlock_NodeHeader *_Header = nullptr;
	GraphicsBlock_NodeInputBoxSection *_Inputs = nullptr;
	GraphicsBlock_NodeOutputBoxSection *_Outputs = nullptr;

public:
	GraphicsBlock_Node(SDL_Renderer *AssociatedRenderer, FC_Font *HeaderFont);

	virtual void CalculateSize(int MaxWidthHint = -1, int MaxHeightHint = -1) override;
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) override;

	// TODO: function to return the NodeHeader
	GraphicsBlock_NodeHeader *Header();
	GraphicsBlock_NodeInputBoxSection *InputSection();
	GraphicsBlock_NodeOutputBoxSection *OutputSection();
};