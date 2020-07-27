#include "NiirdPad.h"
#include <QtWidgets/QApplication>

#include <fstream>
#include <string>
#include <vector>

#include <SDL.h>

#include "TUScriptLexer.h"

#include "ResourceDiscriminator.h"

//#include "DialogueFile.h"

const std::string DemoScript = R"(
||
>>Start hub
{start | save_index encountered, add_flag braq_encountered} You have encountered a large, eyeless beast with six large breasts underneath it. It slowly walks around, sniffing loudly and suckling on the pink stalagmites near it. It doesn't seem to have eyes or teeth.
{encountered | check_flag pc_knowledge_braq encountered knowledgeable} You have encountered one of those thick, long-necked creatures noisily sucking one of the stalagmites.
{knowledgeable} You come across one of the big rocks your recognize as the main source of nutrients for a few species in the region, especially Braqs. In fact one of them, easy to discern as a female of the species thanks to its huge six breasts, is currently feeding off of it, care freely suckling one of the stalagmites.
	[move_closer] (Move closer)
	[ | start_music sejan/fight_weak 70, start_combat braq_victory braq_lost] (Attack)
	[ | end_encounter] (Leave)
||
{move_closer} You start to walk closer to the creature. As you do, it stops sucking the stalagmite and its head snaps in your direction. It sniffs the air and immediately turns its body to face you, remaining completely still. You cautiously advance a bit more, yet when you're just a few steps away, the beast bellows loudly and starts stomping on the ground with its two front legs, flailing its neck wildly from side to side as it keeps making warning noises.
	[calm_braq] (Move even closer) //showif.has_flag.pc_knowledge_braq
	[back_away] (Back away and leave it alone)
	[ | start_music sejan/fight_weak 70, start_combat braq_victory braq_lost] (Attack)
||
{back_away} It clearly doesn't want you anywhere near and it seems ready to attack at any moment. Not wanting to face such a big creature you start taking steps back. When you're far enough, the beast calms down a bit but keeps facing in your direction, grunting and snorting. As you leave, you take one last glance back and see it returning to its business licking that rock.
	[ | end_encounter] (Leave)
||>>Braq victory
{braq_victory | start_music sejan/fight_end 70 0 0 no_loop} You fall to the ground defeated. The creature keeps stomping at the ground in a frenzy, though, so you quickly move away and run in case it tries to trample you. You're not hurt badly other than a few bruises, but you're in no condition to keep fighting, so you settle on continuing your trip and leave the creature with its beloved rock.
	[ | end_encounter] (Leave)
||>>Braq defeat
{braq_lost | start_music sejan/fight_end 70 0 0 no_loop} After delivering that last blow, the beast kneels on the ground and its neck and tail go limp. You get close to check if it's dead, but it's clearly breathing.
	[examine_creature | start_music sejan/map 70 3] (Examine it)
	[ | end_encounter] (Leave)
||>>Examination
{examine_creature} You put a hand on its back and caress it. Its skin is a bit rough and its flesh feels a bit squishy. You also note that it's now pretty sweaty, you guess from the exertion of the fight. You kneel, reach under the beast's thick torso and rub its underbelly between two of its legs from the left set. The skin there is pretty soft, especially on the breasts. A heavy tail covers its rear.
{lift_tail} You move behind it and move its tail with some difficulty. You see a pink puffy slit, but no anus.
	[fuck_braq] (Fuck the creature) //showif.index_is.lift_tail //showif.player_sex.male
	[lift_tail] (Move its tail) //hideif.index_is.lift_tail
	[stop_exam] (Stop)
||
{stop_exam | remove_pic} You pull away and stand back. The confused beast gets back on its feet and slowly goes back to suckling the pink stalagmites.
	[ | end_encounter] (Leave)
||>>Calm Braq (PC has Braq knowledge)
{calm_braq} Knowing the Braqs are very docile by nature you get closer to the stomping creature.<br><br>All bark, no bite. As soon as you're just three steps in front of it and stop, the Braq immediately calms down. It sorts, its two nasal orifices widening, and lowers its neck then sniffs your entire upper body. As it does so, you put a hand on the top of its head and rub it. After a minute is grunts contentedly and relaxes while you caress the sides of its long neck with both hands. It doesn't mind your presence any more.
	[examine_know] (Examine the Braq)
	[ | end_encounter] (Leave)
||
{examine_know} You playfully run your hands all over the Braq's neck, torso and tail, which the beast seems to enjoy. Its skin is a bit rough, yet its flesh feels a bit squishy. It's definitely a meaty creature. Moving down, you touch its legs. Unlike the rest of the body, these are definitely strong and full of very thick muscle, as expected of such a big creature. Finally, you touch the Braq's underbelly. It's pretty soft unlike the rest of its skin, which explains why they rest with their legs protecting their underside, and it's a little bit sweaty. Just for fun, you grab from bellow one of its massive and rather human-looking breasts and lift it. It certainly weights a few kilos. Satisfied, you get up and pat the Braq's neck once more.
{lift_tail_2} You move behind it and move its tail with some difficulty. You see its pink puffy cloaca.
	[sex_with_braq] (Fuck the Braq) //showif.player_sex.male //showif.index_is.lift_tail_2
	[lift_tail_2] (Move its tail) //hideif.index_is.lift_tail_2
	[stop_exam] (Stop)
||)";

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
