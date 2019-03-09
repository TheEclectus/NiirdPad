#include "QSDLPanel.h"

void QSDLPanel::Input()
{
	/*SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		if(Event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
			printf_s("Click!\n");
		else if (Event.type == SDL_EventType::SDL_WINDOWEVENT)
		{
			if (Event.window.event == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED)
			{
				printf_s("Resized to %d by %d pixels.\n", Event.window.data1, Event.window.data2);
			}
		}
	}*/
}

void QSDLPanel::Logic()
{

}

void QSDLPanel::Render()
{
	
}

SDL_Renderer *QSDLPanel::SDLRenderer() const
{
	return _SDLRenderer;
}

void QSDLPanel::ProcessInternal()
{
	Input();
	Logic();

	// TODO: Consider moving the 3 SDL_* and Render() to a separate function.
	SDL_SetRenderDrawColor(_SDLRenderer, _BackgroundColor.r, _BackgroundColor.g, _BackgroundColor.b, 255);
	SDL_RenderClear(_SDLRenderer);
	Render();
	SDL_RenderPresent(_SDLRenderer);
}

QSDLPanel::QSDLPanel(QWidget *parent) :
	QWidget(parent)
{
	this->setAutoFillBackground(true);
	SetBackgroundColor({ 0x1E, 0x1E, 0x1E, 0xFF });

	auto WinID = this->winId();
	_SDLWindow = SDL_CreateWindowFrom((const void*)WinID);
	_SDLRenderer = SDL_CreateRenderer(_SDLWindow, -1, SDL_RENDERER_ACCELERATED);

	//SDL_SetRenderDrawColor(_SDLRenderer, 0, 127, 0, 255);
	//SDL_RenderClear(_SDLRenderer);

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

void QSDLPanel::SetBackgroundColor(const SDL_Color &Color)
{
	_BackgroundColor = Color;

	QPalette NewPalette;
	NewPalette.setColor(QPalette::ColorRole::Background, QColor(Color.r, Color.g, Color.b));
	this->setPalette(NewPalette);
}