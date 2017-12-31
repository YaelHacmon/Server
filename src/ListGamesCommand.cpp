#include "../include/ListGamesCommand.h"
#include "../include/GamesInfoLists.h"
#include "../include/CommunicationManager.h"
#include <unistd.h> //for close()
#include <cstdlib> //for atoi()

using namespace std;

ListGamesCommand::ListGamesCommand() {}

void ListGamesCommand::execute(vector<string> args) {
	//get given client's sd (first argument)
	int client = atoi(args[0].c_str());

	//get string of waiting games
	string list = GamesInfoLists::getInstance()->listWaitingGames();

	//send list via server
	//if problem occured - close client (exiting is done right after, anyway)
	if (!CommunicationManager::getInstance()->writeString(list, client)) {
		close(client);
	}

	//end thread via return from method
}
