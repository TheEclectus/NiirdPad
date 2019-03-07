#pragma once

#include <QTimer>
#include <QWidget>

#include <SDL.h>

class QSDLPanel : public QWidget
{
	Q_OBJECT

private:
	SDL_Color _BackgroundColor = {0, 127, 0, 255};

	QTimer *_RefreshTimer = nullptr;
	SDL_Window *_SDLWindow = nullptr;
	SDL_Renderer *_SDLRenderer = nullptr;

	void Input();
	void Logic();
	void Render();

private slots:
	void ProcessInternal();

public:
	QSDLPanel(QWidget *parent);
	virtual ~QSDLPanel();
};
