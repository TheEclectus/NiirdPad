#include "QSDLPanel.h"

QSDLPanel::QSDLPanel(QWidget *parent) :
	QWidget(parent)
{
	_RefreshTimer = new QTimer(this);
	_RefreshTimer->setInterval(1000 / 30);

	auto WinID = this->winId();
	_SDLWindow = SDL_CreateWindowFrom((const void*)WinID);

	//_RenderTarget = GPU_CreateTargetFromWindow(SDL_GetWindowID(_SDLWindow));

	auto SDLWinID = SDL_GetWindowID(_SDLWindow);
	GPU_SetInitWindow(SDLWinID);
	_RenderTarget = GPU_Init(500, 500, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		//GPU_Init(this->width(), this->height(), SDL_INIT_VIDEO);
	
	GPU_ClearRGB(_RenderTarget, 0, 0, 0);
	GPU_Flip(_RenderTarget);
}

QSDLPanel::~QSDLPanel()
{

}
