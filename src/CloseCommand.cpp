#include "CloseCommand.h"

using namespace std;

CloseCommand::CloseCommand(Server& s, GamesInfoLists& list): Command(s, list) {}


void CloseCommand::execute(vector<string> args) {
	//get client1 - first argument
	int client1_sd = atoi(args[0].c_str());

	//find game by client1's sd (arbitrary)
	GameInfo g = list_.findGame(client1_sd);

	//close sockets - we don't know who is which, so close using socket despcriptors from GameInfo
	server_.closeClient(g.getClientA());
	server_.closeClient(g.getClientB());

	//remove from list - by client 1 (arbitrary, it doens't matter)
	list_.removeGame(client1_sd);

	//kill thread
	pthread_exit(NULL);
}
