#include "CloseCommand.h"

using namespace std;

CloseCommand::CloseCommand(Server& s, GamesInfoLists& list): Command(s, list) {}


void CloseCommand::execute(vector<string> args) {
	//get client1 - first argument
	int client1_sd = atoi(args[0].c_str());

	//find game by client1's sd (arbitrary)
	GameInfo g = list_.findGame(client1_sd);

	//make game in ending status - handling methods will know that game ended
	//(will know to break the loop - when game is in ending status)
	g.endGame();
}
