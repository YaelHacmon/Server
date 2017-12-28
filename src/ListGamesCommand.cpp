#include "ListGamesCommand.h"
#include <cstdlib> //for atoi()

using namespace std;

ListGamesCommand::ListGamesCommand(Server& s, GamesInfoLists& list): Command(s, list) {}

void ListGamesCommand::execute(vector<string> args) {
	//get given client's sd (first argument)
	int client = atoi(args[0].c_str());

	//get string of waiting games
	string list = list_.listWaitingGames();

	//send list via server
	//if problem occured - close client (exiting is done right after, anyway)
	if (!server_.writeString(list, client)) {
		close(client);
	}

	//end thread via return from method
}
