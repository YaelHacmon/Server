#include "../include/StartGameCommand.h"
#include "../include/GamesInfoLists.h"
#include "../include/CommunicationManager.h"
#include <cstdlib> //for atoi()

using namespace std;

StartGameCommand::StartGameCommand() {}

void StartGameCommand::execute(vector<string> args) {
	//get given game name (first argument) and client's sd (second argument)
	string name = args[0];
	int clientA = atoi(args[1].c_str());

	//call list to start new game. if a game with the given name exists, 1 will be returned. 0 if method was successful
	//if starting a new game was not successful
	if (GamesInfoLists::getInstance()->startNewGame(name, clientA)) {
		//try to write -1 (error code) to client - notify client that initialization is impossible
		CommunicationManager::getInstance()->writeNum(-1, clientA); //returned value does not matter to us (if client disconnected - his problem :) )

	} else if (!CommunicationManager::getInstance()->writeNum(0, clientA)) {
		//otherwise - send 0 to client, to show initialization worked
		//if writing failed, client disconnected - remove the game just created
		GamesInfoLists::getInstance()->removeGame(clientA);
	}
}
