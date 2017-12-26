/*
 * StartGameCommand.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: yael
 */

#include "../include/StartGameCommand.h"


using namespace std;

StartGameCommand::StartGameCommand(Server& s, GamesInfoLists& list): Command(s, list) {}

void StartGameCommand::execute(vector<string> args) {
	//get given game name (first argument) and client's sd (second argument)
	string name = args[0];
	int clientB = atoi(args[1].c_str());

	//call list to start new game
	list_.startNewGame(name, clientB);
}
