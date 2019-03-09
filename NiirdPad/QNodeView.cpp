#include "QNodeView.h"

void QNodeView::Input()
{

}

void QNodeView::Logic()
{

}

void QNodeView::Render()
{
	RenderBackground();

	RenderForeground();
}

QNodeView::QNodeView(QWidget *Parent) :
	QSDLPanel(Parent)
{

}