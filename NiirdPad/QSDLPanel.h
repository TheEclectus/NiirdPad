#pragma once

#include <QTimer>
#include <QWidget>

#include <SDL.h>

class QSDLPanel : public QWidget
{
	Q_OBJECT

private:
	QTimer *_RefreshTimer;

	SDL_Window *_SDLWindow;
public:
	QSDLPanel(QWidget *parent);
	~QSDLPanel();
};
