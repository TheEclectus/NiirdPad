#pragma once

#include <vector>

#include <SDL.h>

// Node.h
class Node;
class ANub;
class NubOutput;

struct QNodeViewCamera
{
	SDL_Rect ViewBox = { 0, 0, 0, 0 };	// Center position and size.
	float Scale = 1.0f;
};

struct QNodeViewInputState
{
	static constexpr unsigned int DRAG_THRESHOLD = 10;

	bool bLeftMouseDown = false;
	bool bRightMouseDown = false;

	bool bDraggingNodes = false;

	SDL_Point Position = { -1, -1 };
	SDL_Point DownPosition[3] = { { -1, -1 }, { -1, -1 }, { -1, -1 } };

	std::vector<Node*> SelectedNodes;
	ANub *MousedOverNub = nullptr;
	ANub *DraggingNub = nullptr;

	/// <summary>
	/// Resets all applicable input state information:
	/// <para>- Currently selected nodes</para>
	/// </summary>
	void Reset();
};