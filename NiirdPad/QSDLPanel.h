#pragma once

#include <QTimer>
#include <QWidget>

#include <SDL.h>

/// <summary>
/// A QWidget whose hWnd is used to render an SDL context, and register relevant
/// user events.
/// </summary>
/// <remarks>
/// EVENTS: SDL events -- the mouse and keyboard events in particular -- are extremely
/// unreliable here. Therefore, I've decided to instead forward Qt's widget-space events 
/// to SDL as user events:
///		EVENT_MOUSEDOWN - A mouse-down event.
///			.data1	- x position in widget
///			.data2	- y position in widget
///			.code	- the Qt::MouseButton that was pressed
///		EVENT_MOUSEUP - A mouse-up event.
///			.data1	- x position in widget
///			.data2	- y position in widget
///			.code	- the Qt::MouseButton that was released
///		EVENT_MOUSEMOVE - A mouse movement event.
///			.data1	- x position in widget
///			.data2	- y position in widget
///			.code	- the Qt::MouseButtons that are being held while the mouse was moved
/// Note that it is NOT QSDLPanel's responsibility to implement any application-specific
/// features, like down mouse click-drags are handled, or mouse movement deltas.
/// </remarks>
class QSDLPanel : public QWidget
{
	Q_OBJECT

private:
	SDL_Color _BackgroundColor = {0, 0, 0, 255};
	SDL_Point _LastMousePosition = { INT32_MIN, INT32_MIN };

	QTimer *_RefreshTimer = nullptr;

	void _RegisterEvents();

protected:
	// .data1 = x, .data2 = y, .code = Qt::MouseButton
	uint32_t EVENT_MOUSEDOWN;

	// .data1 = x, .data2 = y, .code = Qt::MouseButton
	uint32_t EVENT_MOUSEUP;

	// .data1 = x, .data2 = y, .code = Qt::MouseButtons OR'd together (buttons held)
	uint32_t EVENT_MOUSEMOVE;

	// .data1 = x, .data2 = y, .code = Qt::MouseButton
	uint32_t EVENT_MOUSEDOUBLECLICK;

	SDL_Window *_SDLWindow = nullptr;
	SDL_Renderer *_SDLRenderer = nullptr;

	bool _bWarpingMouse = false;	// Send a delta of zero with any mouse movement events

	virtual void Input();
	virtual void Logic();
	virtual void Render();

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
	void ProcessInternal();

public:
	QSDLPanel(QWidget *parent);
	virtual ~QSDLPanel();

	/*
		TODO: See if pausing the rendering timer when the window loses focus or is
		resized will stop the flickering, and potentially make setting the background
		on the underlying widget unnecessary.
	*/
	/// <summary>
	/// Sets the clear (background) color of _SDLRenderer and the underlying widget.
	/// </summary>
	/// <param name="Color"></param>
	void SetBackgroundColor(const SDL_Color &Color);

	const SDL_Point &GetMousePosition() const;

	SDL_Renderer *SDLRenderer() const;
};
