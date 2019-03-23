#pragma once

#include <SDL_rect.h>

/// <summary>
/// A graphical building block class. Has faculties for defining the bounding box
/// of a graphical object based on its own criteria, and that of blocks within it.
/// </summary>
class AGraphicsBlock
{
protected:
	/// <summary>
	/// Defines the maximum size of the block. {-1, -1} means no limit.
	/// </summary>
	SDL_Rect _MaximumSize = { -1, -1 };
	/// <summary>
	/// Defines the minimum size of a block.
	/// </summary>
	SDL_Rect _MinimumSize = { 0, 0 };
public:
	void CalculateSize();
};