#include "QSDLPanel.h"

void QSDLPanel::ProcessInternal()
{
	SDL_SetRenderDrawColor(_SDLRenderer, _BackgroundColor.r, _BackgroundColor.g, _BackgroundColor.b, 255);
	SDL_RenderClear(_SDLRenderer);

	SDL_RenderPresent(_SDLRenderer);
}

QSDLPanel::QSDLPanel(QWidget *parent) :
	QWidget(parent)
{
	auto WinID = this->winId();
	_SDLWindow = SDL_CreateWindowFrom((const void*)WinID);
	_SDLRenderer = SDL_CreateRenderer(_SDLWindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(_SDLRenderer, 0, 127, 0, 255);
	SDL_RenderClear(_SDLRenderer);

	_RefreshTimer = new QTimer(this);
	_RefreshTimer->setInterval(1000 / 30);
	this->connect(_RefreshTimer, &QTimer::timeout, this, &QSDLPanel::ProcessInternal);
	_RefreshTimer->start();
}

QSDLPanel::~QSDLPanel()
{
	SDL_DestroyRenderer(_SDLRenderer);
	SDL_DestroyWindow(_SDLWindow);
}