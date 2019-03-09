#include "QNodeView.h"

void QNodeView::Input()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_EventType::SDL_WINDOWEVENT)
		{
			SDL_WindowEvent &WinEvent = Event.window;
			if (WinEvent.type == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED)
			{
				int NewWidth = WinEvent.data1;
				int NewHeight = WinEvent.data2;

				_Camera.ViewBox.w = NewWidth;
				_Camera.ViewBox.h = NewHeight;
			}
		}
	}
}

void QNodeView::Logic()
{

}

void QNodeView::Render()
{
	RenderBackground();

	RenderForeground();
}

void QNodeView::RenderBackground()
{

}

void QNodeView::RenderForeground()
{

}

QNodeView::QNodeView(QWidget *Parent) :
	QSDLPanel(Parent)
{

}