#include "QNodeView.h"

void QNodeView::Input()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		if (Event.type == SDL_EventType::SDL_WINDOWEVENT)
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
		else if (Event.type == SDL_EventType::SDL_MOUSEBUTTONDOWN)
		{
			SDL_MouseButtonEvent &MouseEvent = Event.button;
			if (MouseEvent.button == SDL_BUTTON_LEFT)
			{
				_InputState.bLeftMouseDown = true;
				_InputState.DownPosition = { MouseEvent.x, MouseEvent.y };
				printf_s("Left mouse down.\n");
			}
			else if (MouseEvent.button == SDL_BUTTON_RIGHT)
			{
				_InputState.bRightMouseDown = true;
				_InputState.DownPosition = { MouseEvent.x, MouseEvent.y };
				printf_s("Right mouse down.\n");
			}
		}
		// Handle the 'click' event here.
		else if (Event.type == SDL_EventType::SDL_MOUSEBUTTONUP)
		{
			SDL_MouseButtonEvent &MouseEvent = Event.button;
			if (MouseEvent.button == SDL_BUTTON_LEFT)
			{
				_InputState.bLeftMouseDown = false;
				_InputState.DownPosition = { -1, -1 };
				printf_s("Left mouse up.\n");
			}
			else if (MouseEvent.button == SDL_BUTTON_RIGHT)
			{
				_InputState.bRightMouseDown = false;
				_InputState.DownPosition = { -1, -1 };
				printf_s("Right mouse up.\n");
			}
		}
		// TODO: Doesn't seem to fire unless the mouse is held down. For the time being, this isn't a problem, but mouse down should still be checked for regardless.
		else if (Event.type == SDL_EventType::SDL_MOUSEMOTION)
		{
			if (_InputState.bLeftMouseDown || _InputState.bRightMouseDown)
			{
				SDL_MouseMotionEvent &MouseEvent = Event.motion;

				// NEXTTIME: Diffs are always how far the frame was dragged last time.

				int XDiff = _InputState.DownPosition.x - MouseEvent.x;
				int YDiff = _InputState.DownPosition.y - MouseEvent.y;

				_Camera.ViewBox.x += XDiff;
				_Camera.ViewBox.y += YDiff;
				_InputState.DownPosition = { MouseEvent.x, MouseEvent.y };
				printf_s("Diff:%d,%d\n", XDiff, YDiff);
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