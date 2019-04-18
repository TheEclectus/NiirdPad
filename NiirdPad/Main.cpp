#include "NiirdPad.h"
#include <QtWidgets/QApplication>

#include <SDL.h>

#include "TUScriptLexer.h"

const std::string DemoScript = R"(>>Creature is encountered when accepting to help Breib get rid of its rival in the beach cave.

||>>test comment
{start | play_sound sejan/naigad_splash 70, save_index rematch, remove_flag breirb_rival_known, add_flag breirb_rival_encountered} A massive creature emerges from the water. Only half of its body is visible, but judging by the shape under the water you calculate it's roughly six meters long. Water falls down its maw and the coral-like crown around its head as it observes you.
{ugly_mofo | play_sound sejan/caenemung_roar 100} The creature roars defiantly and opens its maw ready to strike. Fight seems inevitable.
	[test] Fuck the niird.
	[ | start_music sejan/fight_strong 80, start_combat defeat victory] (Attack)
	[can_we_talk] Hi, can we talk? //hideif.clicked //showif.charisma.6
	[ugly_mofo] You're one ugly motherfucker. //showif.will.8 //hideif.clicked
||)";

int main(int argc, char *argv[])
{
	pegtl::string_input<> In(DemoScript, "sus");
	TUScript::State State;

	try 
	{
		bool Result = pegtl::parse<TUScript::Grammar, TUScript::Action>(In, State);
		if (!Result)
		{
			printf_s("Parsing failed.\n");
		}
	}
	catch (std::exception Exception)
	{
		printf_s("%s\n", Exception.what());
	}

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
	QApplication a(argc, argv);
	NiirdPad w;
	w.show();
	return a.exec();
}
