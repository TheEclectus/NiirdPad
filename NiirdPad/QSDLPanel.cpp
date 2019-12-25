#include "QSDLPanel.h"

#include <QMouseEvent>

void QSDLPanel::_RegisterEvents()
{
	uint32_t UserEventsStart = SDL_RegisterEvents(3);
	EVENT_MOUSEDOWN		= UserEventsStart + 0;
	EVENT_MOUSEUP		= UserEventsStart + 1;
	EVENT_MOUSEMOVE		= UserEventsStart + 2;
	EVENT_MOUSEDOUBLECLICK	= UserEventsStart + 3;
}

void QSDLPanel::Input()
{

}

void QSDLPanel::Logic()
{

}

void QSDLPanel::Render()
{
	
}

void QSDLPanel::mousePressEvent(QMouseEvent *event)
{
	SDL_Event NewEvent;
	SDL_UserEvent &User = NewEvent.user;

	memset(&NewEvent, 0, sizeof(SDL_Event));

	User.type = EVENT_MOUSEDOWN;
	User.code = event->button();
	User.data1 = reinterpret_cast<void*>(event->x());
	User.data2 = reinterpret_cast<void*>(event->y());

	_LastMousePosition = { event->x(), event->y() };

	SDL_PushEvent(&NewEvent);
}

void QSDLPanel::mouseReleaseEvent(QMouseEvent *event)
{
	SDL_Event NewEvent;
	SDL_UserEvent &User = NewEvent.user;
	
	memset(&NewEvent, 0, sizeof(SDL_Event));

	User.type = EVENT_MOUSEUP;
	User.code = event->button();
	User.data1 = reinterpret_cast<void*>(event->x());
	User.data2 = reinterpret_cast<void*>(event->y());

	_LastMousePosition = { event->x(), event->y() };

	SDL_PushEvent(&NewEvent);
}

void QSDLPanel::mouseMoveEvent(QMouseEvent *event)
{
	SDL_Event NewEvent;
	SDL_UserEvent &User = NewEvent.user;

	memset(&NewEvent, 0, sizeof(SDL_Event));

	User.type = EVENT_MOUSEMOVE;
	User.code = event->buttons();

	int DeltaX = event->x() - _LastMousePosition.x;
	int DeltaY = event->y() - _LastMousePosition.y;

	if ((_LastMousePosition.x == INT32_MIN && _LastMousePosition.y == INT32_MIN) || _bWarpingMouse == true)
	{
		User.data1 = nullptr;
		User.data2 = nullptr;

		if (_bWarpingMouse) _bWarpingMouse = false;
	}
	else
	{
		User.data1 = reinterpret_cast<void*>(DeltaX);
		User.data2 = reinterpret_cast<void*>(DeltaY);
	}

	_LastMousePosition = { event->x(), event->y() };

	SDL_PushEvent(&NewEvent);
}

void QSDLPanel::mouseDoubleClickEvent(QMouseEvent *event)
{
	SDL_Event NewEvent;
	SDL_UserEvent &User = NewEvent.user;

	memset(&NewEvent, 0, sizeof(SDL_Event));

	User.type = EVENT_MOUSEDOUBLECLICK;
	User.code = event->button();
	User.data1 = reinterpret_cast<void*>(event->x());
	User.data2 = reinterpret_cast<void*>(event->y());

	_LastMousePosition = { event->x(), event->y() };

	SDL_PushEvent(&NewEvent);
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
	_RegisterEvents();

	this->setMouseTracking(true);

	this->setAutoFillBackground(true);
	SetBackgroundColor({ 42, 42, 42, 0xFF });

	auto WinID = this->winId();
	_SDLWindow = SDL_CreateWindowFrom((const void*)WinID);
	_SDLRenderer = SDL_CreateRenderer(_SDLWindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(_SDLRenderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);

	//SDL_SetRenderDrawColor(_SDLRenderer, 0, 127, 0, 255);
	//SDL_RenderClear(_SDLRenderer);

	_RefreshTimer = new QTimer(this);
	_RefreshTimer->setInterval(1000 / 60);
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

const SDL_Point &QSDLPanel::GetMousePosition() const
{
	return _LastMousePosition;
}