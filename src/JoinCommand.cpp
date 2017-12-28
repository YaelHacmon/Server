#include "JoinCommand.h"
#include <cstdlib> //for atoi()

using namespace std;

JoinCommand::JoinCommand(Server& s, GamesInfoLists& list):  Command(s, list) {}


void JoinCommand::execute(vector<string> args) {
	//get given game name (first argument) and client's sd (second argument)
	string name = args[0];
	int clientB = atoi(args[1].c_str());

	//join game
	GameInfo g = list_.joinGame(name, clientB);

	//if returned GameInfo is not the null game (clientA=-2, name=empty string) - ask server to communicate given game
	if (g != GameInfo("", -2)) {
		server_.handleGame(g);
	}

	//end thread via return from method - game does not exist or is already being played
}

