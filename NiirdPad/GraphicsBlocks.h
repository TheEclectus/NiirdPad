#pragma once

#include <vector>

#include <SDL_rect.h>

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
	/// Defines the maximum size of the block. {-1, -1} means no limit.
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
	AGraphicsBlock(SDL_Rect &MaximumSize = { 0, 0, -1, -1 }, SDL_Rect &MinimumSize = { 0, 0, 0, 0 });
	virtual ~AGraphicsBlock();

	/// <summary>
	/// Adds an AGraphicsBlock as a child, ensuring it will be properly assigned this as its parent and cleaned up on destruction.
	/// </summary>
	/// <param name="Child">The AGraphicsBlock to add as a child.</param>
	void AddChild(AGraphicsBlock *Child);

	/// <summary>
	/// Calculates the bounds of the block based on the return values of its children and
	/// minimum/maximum sizes. 
	/// </summary>
	virtual void CalculateSize() = 0;
};