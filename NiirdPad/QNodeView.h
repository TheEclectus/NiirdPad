#pragma once

#include "QSDLPanel.h"

class QNodeView : public QSDLPanel
{
protected:


	virtual void Input() override;
	virtual void Logic() override;
	virtual void Render() override;

	void RenderBackground();

public:
	QNodeView(QWidget *Parent);
};