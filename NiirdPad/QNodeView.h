#pragma once

#include "QSDLPanel.h"

struct QNodeViewCamera
{
	SDL_Rect ViewBox;	// Center position and size.
	float Scale = 1.0f;
};

class QNodeView : public QSDLPanel
{
protected:
	SDL_Color _GridLineColor = { 0x1E, 0x1E, 0x1E, 0xFF };
	QNodeViewCamera _Camera;

	virtual void Input() override;
	virtual void Logic() override;
	virtual void Render() override;

	void RenderBackground();
	void RenderForeground();

public:
	QNodeView(QWidget *Parent);
};