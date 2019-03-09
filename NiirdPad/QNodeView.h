#pragma once

#include "QSDLPanel.h"

class QNodeView : public QSDLPanel
{
protected:
	SDL_Color _GridLineColor = { 0x1E, 0x1E, 0x1E, 0xFF };

	virtual void Input() override;
	virtual void Logic() override;
	virtual void Render() override;

	void RenderBackground();
	void RenderForeground();

public:
	QNodeView(QWidget *Parent);
};