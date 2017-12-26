/*
 * ListGamesCommand.cpp
 *      Author: yael
 */
#include "ListGamesCommand.h"

using namespace std;

ListGamesCommand::ListGamesCommand(Server& s, GamesInfoLists& list): Command(s, list) {}

void ListGamesCommand::execute(vector<string> args) {
	//get given client's sd (first argument)
	int client = atoi(args[0].c_str());

	//get string of waiting games
	string list = list_.listWaitingGames();

	//send list via server
	server_.writeString(list, client);

	//end thread - we finished our work here :)
	pthread_exit(NULL);
}
