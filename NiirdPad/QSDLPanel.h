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

private slots:
	void RenderInternal();

public:
	QSDLPanel(QWidget *parent);
	~QSDLPanel();
};
