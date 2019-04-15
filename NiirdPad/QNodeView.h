#pragma once

#include "QSDLPanel.h"

#include "FontStore.h"

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
};