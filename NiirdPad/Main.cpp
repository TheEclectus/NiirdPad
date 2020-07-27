#include "NiirdPad.h"
#include <QtWidgets/QApplication>

#include <fstream>
#include <string>
#include <vector>

#include <SDL.h>

#include "TUScriptLexer.h"

#include "ResourceDiscriminator.h"

//#include "DialogueFile.h"

//const std::string DemoScript = R"(	[test] Fuck the niird.
//	[ | start_music sejan/fight_strong 80, start_combat defeat victory] (Attack)
//	[can_we_talk] Hi, can we talk? //hideif.clicked //showif.charisma.6
//	[ugly_mofo] You're one ugly motherfucker. //showif.will.8 //hideif.clicked)";

int main(int argc, char *argv[])
{
	qRegisterMetaType<std::vector<std::string>>("std::vector<std::string>");
	qRegisterMetaType<RawProjectFile>("RawProjectFile");
	qRegisterMetaType<ResourceDiscriminator::Results>("Results");
	//qRegisterMetaType<DialogueFile*>("DialogueFile");

	//ResourceDiscriminator::Results::Character TestChar("Z:\\Teraurge\\Teraurge\\database\\characters\\braq_m");
	//ResourceDiscriminator TestRes({ "Z:\\Teraurge\\Teraurge\\database" }, ResourceDiscriminator::SelectionType::FullDatabaseFolder);

	std::ifstream InFile{ "Z:\\Teraurge\\Teraurge\\database\\characters\\braq_m\\diag.txt" };
	std::string SrcString{ std::istreambuf_iterator<char>(InFile), std::istreambuf_iterator<char>() };

	pegtl::string_input<> In(SrcString, "sus");
	TUScript::State State;

	/*try 
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
	}*/

	SDL_SetMainReady();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
	QApplication a(argc, argv);
	NiirdPad w;
	w.show();
	return a.exec();
}
