#pragma once

#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>
#include "SDL_FontCache.h"

/// <summary>
/// A graphical building block class. Has faculties for defining the bounding box
/// of a graphical object based on its own criteria, and that of blocks within it.
/// </summary>
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

	/// <summary>
	/// Defines the maximum size of the block. -1 in either width or height field means no limit.
	/// </summary>
	SDL_Rect _MaximumSize;

	/// <summary>
	/// Defines the minimum size of a block.
	/// </summary>
	SDL_Rect _MinimumSize;

	/// <summary>
	/// The calculated dimensions and offset from the parent (uppermost block is always 0,0 
	/// position).
	/// </summary>
	SDL_Rect _CalculatedBounds = { 0, 0, 0, 0 };

public:
	AGraphicsBlock(const SDL_Rect &MaximumSize = { 0, 0, -1, -1 }, const SDL_Rect &MinimumSize = { 0, 0, 0, 0 });
	virtual ~AGraphicsBlock();

	/// <summary>
	/// Adds an AGraphicsBlock as a child, ensuring it will be properly assigned this as its parent and cleaned up on destruction.
	/// </summary>
	/// <param name="Child">The AGraphicsBlock to add as a child.</param>
	void AddChild(AGraphicsBlock *Child);

	/// <summary>
	/// Calculates the bounds of the block based on the return values of its children and
	/// minimum/maximum sizes. 
	/// Should be called on _ParentBlock when size-affecting changes are made.
	/// </summary>
	virtual void CalculateSize() = 0;

	/// <summary>
	/// Renders the AGraphicsBlock to an SDL_Renderer.
	/// </summary>
	/// <param name="SDLRenderer"></param>
	/// <param name="Position"></param>
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) = 0;
};

class GraphicsBlock_Text : public AGraphicsBlock
{
protected:
	std::string _Text;
public:
	GraphicsBlock_Text();
	
	GraphicsBlock_Text &SetText(const std::string &Text);
	GraphicsBlock_Text &SetText(const std::string &Text, const int MaxWidth);
	const std::string &GetText() const;
};

class GraphicsBlock_Node : public AGraphicsBlock
{
public:
	GraphicsBlock_Node();

	virtual void CalculateSize() override;
	virtual void Render(SDL_Renderer *SDLRenderer, SDL_Point Position) override;
};