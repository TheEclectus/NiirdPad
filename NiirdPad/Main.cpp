#include "NiirdPad.h"
#include <QtWidgets/QApplication>

#include <SDL.h>

int main(int argc, char *argv[])
{
	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
	QApplication a(argc, argv);
	NiirdPad w;
	w.show();
	return a.exec();
}
