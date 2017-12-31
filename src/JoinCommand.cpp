#include "../include/JoinCommand.h"
#include "../include/GamesInfoLists.h"
#include "../include/CommunicationManager.h"
#include <cstdlib> //for atoi()

using namespace std;

JoinCommand::JoinCommand() {}


void JoinCommand::execute(vector<string> args) {
	//get given game name (first argument) and client's sd (second argument)
	string name = args[0];
	int clientB = atoi(args[1].c_str());

	//join game
	GameInfo g = GamesInfoLists::getInstance()->joinGame(name, clientB);

	//if returned GameInfo is not the null game (clientA=-2, name=empty string) - ask server to communicate given game
	if (g != GameInfo("", -2)) {
		CommunicationManager::getInstance()->handleGame(g);
	}

	//end thread via return from method - game does not exist or is already being played
}

