#pragma once

#include <vector>

#include <QMenu>

#include "FontStore.h"
#include "QSDLPanel.h"
#include "ScriptEngine.h"

// Node.h
class Node;

// NiirdPad.h
class NiirdPad;

struct QNodeViewCamera
{
	SDL_Rect ViewBox = { 0, 0, 0, 0 };	// Center position and size.
	float Scale = 1.0f;
};

struct QNodeViewInputState
{
	bool bLeftMouseDown = false;
	bool bRightMouseDown = false;

	SDL_Point Position = { -1, -1 };
	SDL_Point DownPosition[3] = { { -1, -1 }, { -1, -1 }, { -1, -1 } };

	std::vector<Node*> SelectedNodes;

	/// <summary>
	/// Resets all applicable input state information:
	/// <para>- Currently selected nodes</para>
	/// </summary>
	void Reset();
};

class QNodeView : public QSDLPanel
{
protected:
	SDL_Color _GridLineColor = { 50, 50, 50, 0xFF };
	QNodeViewCamera _Camera;
	QNodeViewInputState _InputState;
	FontStore _FontStore;
	// HACK: Figure out a more-elegant solution than this shite.
	ScriptEngine *_Engine = nullptr;
	NiirdPad *_Parent = nullptr;

	std::vector<Node*> _Nodes;	// Order changes frequently, most-recently-clicked Node at front.

	virtual void Input() override;
	virtual void Logic() override;
	virtual void Render() override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

	void RenderBackground();
	void RenderForeground();

public:
	QNodeView(QWidget *Parent);
	const FontStore &FontStore() const;

	// HACK: So you can't miss the HACK label for setting a crucial pointer outside the constructor, fuckhead
	void SetEngine(ScriptEngine *Engine);

	// HACK: hee hee hoo hoo ho ho it's ME AGAIN, THE HACK LABEL >:D
	void SetNiirdPad(NiirdPad *NP);
};