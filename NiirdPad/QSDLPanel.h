#pragma once

#include <QTimer>
#include <QWidget>

#include <SDL.h>

class QSDLPanel : public QWidget
{
	Q_OBJECT

private:
	SDL_Color _BackgroundColor = {0, 0, 0, 255};

	QTimer *_RefreshTimer = nullptr;
	SDL_Window *_SDLWindow = nullptr;
	SDL_Renderer *_SDLRenderer = nullptr;

protected:
	virtual void Input();
	virtual void Logic();
	virtual void Render();

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
};
