#include "QSDLPanel.h"

QSDLPanel::QSDLPanel(QWidget *parent) :
	QWidget(parent)
{
	_RefreshTimer = new QTimer(this);
	_RefreshTimer->setInterval(1000 / 30);

	auto WinID = this->winId();
	_SDLWindow = SDL_CreateWindowFrom((const void*)WinID);
}

QSDLPanel::~QSDLPanel()
{

}
