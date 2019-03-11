#pragma once

#include "QSDLPanel.h"

struct QNodeViewCamera
{
	SDL_Rect ViewBox = { 0, 0, 0, 0 };	// Center position and size.
	float Scale = 1.0f;
};

struct QNodeViewInputState
{
	bool bLeftMouseDown = false;
	bool bRightMouseDown = false;

	/// <summary>
	/// Resets all applicable input state information:
	/// <para>- Currently selected nodes</para>
	/// </summary>
	void Reset();
};

class QNodeView : public QSDLPanel
{
protected:
	SDL_Color _GridLineColor = { 0x33, 0x33, 0x33, 0xFF };
	QNodeViewCamera _Camera;
	QNodeViewInputState _InputState;

	virtual void Input() override;
	virtual void Logic() override;
	virtual void Render() override;

	void RenderBackground();
	void RenderForeground();

public:
	QNodeView(QWidget *Parent);
};