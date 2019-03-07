#pragma once

#include <QTimer>
#include <QWidget>

#include <SDL_gpu.h>

class QSDLPanel : public QWidget
{
	Q_OBJECT

private:
	QTimer *_RefreshTimer;

	SDL_Window *_SDLWindow;
	GPU_Target *_RenderTarget;
public:
	QSDLPanel(QWidget *parent);
	~QSDLPanel();
};
