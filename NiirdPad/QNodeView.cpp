#include "QNodeView.h"

void QNodeView::Input()
{
	// TODO: Consider pros and cons of flushing event cache, sans user events.

	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		if (Event.type == EVENT_MOUSEDOWN)
		{
			printf("MouseDown (%d, %d)\n", reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2));
		}
		else if (Event.type == EVENT_MOUSEUP)
		{
			printf("MouseUp (%d, %d)\n", reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2));
		}
		else if (Event.type == EVENT_MOUSEMOVE)
		{
			printf("MouseMove [%c%c%c] (%d, %d)\n", Event.user.code & Qt::MouseButton::LeftButton ? 'L' : ' ', Event.user.code & Qt::MouseButton::MiddleButton ? 'M' : ' ', Event.user.code & Qt::MouseButton::RightButton ? 'R' : ' ', reinterpret_cast<int>(Event.user.data1), reinterpret_cast<int>(Event.user.data2));
		}

		else if (Event.type == SDL_EventType::SDL_WINDOWEVENT)
		{
			SDL_WindowEvent &WinEvent = Event.window;
			if (WinEvent.event == SDL_WindowEventID::SDL_WINDOWEVENT_RESIZED)
			{
				int NewWidth = WinEvent.data1;
				int NewHeight = WinEvent.data2;

				_Camera.ViewBox.w = NewWidth;
				_Camera.ViewBox.h = NewHeight;

				printf_s("Camera adjusted to %dx%d.\n", _Camera.ViewBox.w, _Camera.ViewBox.h);
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

void QNodeView::mousePressEvent(QMouseEvent *event)
{
	QSDLPanel::mousePressEvent(event);
}

void QNodeView::mouseReleaseEvent(QMouseEvent *event)
{
	QSDLPanel::mouseReleaseEvent(event);
}

void QNodeView::mouseMoveEvent(QMouseEvent *event)
{
	QSDLPanel::mouseMoveEvent(event);
}

void QNodeView::RenderBackground()
{
	const int GridSize = 120;

	SDL_Renderer *Renderer = SDLRenderer();

	SDL_Point TopLeft = { _Camera.ViewBox.x - (_Camera.ViewBox.w / 2), _Camera.ViewBox.y - (_Camera.ViewBox.h / 2) };
	int XStartOffset = TopLeft.x % GridSize;
	int YStartOffset = TopLeft.y % GridSize;

	SDL_SetRenderDrawColor(Renderer, _GridLineColor.r, _GridLineColor.g, _GridLineColor.b, _GridLineColor.a);
	// TODO: Draw lines in batches with SDL_RenderDrawLines().
	for (int i = 0; i <= _Camera.ViewBox.w / GridSize; i++)
	{
		int XPosition = (i * GridSize) - XStartOffset;
		SDL_RenderDrawLine(Renderer, XPosition, 0, XPosition, _Camera.ViewBox.h);
	}

	for (int i = 0; i <= _Camera.ViewBox.h / GridSize; i++)
	{
		int YPosition = (i * GridSize) - YStartOffset;
		SDL_RenderDrawLine(Renderer, 0, YPosition, _Camera.ViewBox.w, YPosition);
	}
}

void QNodeView::RenderForeground()
{
	SDL_Point TopLeft = { _Camera.ViewBox.x - (_Camera.ViewBox.w / 2), _Camera.ViewBox.y - (_Camera.ViewBox.h / 2) };

	SDL_Renderer *Renderer = SDLRenderer();

	SDL_SetRenderDrawColor(Renderer, 0xFF, 0x00, 0x00, 0xFF);

	// Top-left is 0,0
	// Scene space 0,0, offset -30,20, target is 
	int XOffset = (_Camera.ViewBox.w / 2);
	int YOffset = (_Camera.ViewBox.h / 2);

	SDL_Rect r = { XOffset, YOffset, 5, 5 };
	SDL_RenderFillRect(Renderer, &r);
}

QNodeView::QNodeView(QWidget *Parent) :
	QSDLPanel(Parent)
{

}