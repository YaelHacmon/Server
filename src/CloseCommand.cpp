#include "../include/CloseCommand.h"
#include "../include/GamesInfoLists.h"
#include <unistd.h> //for close()
#include <cstdlib> //for atoi()

using namespace std;

CloseCommand::CloseCommand() {}


void CloseCommand::execute(vector<string>& args, pthread_t& tid) {
	//get client1 - first argument
	int client1_sd = atoi(args[0].c_str());

	//find game by client1's sd (arbitrary)
	GameInfo g = GamesInfoLists::getInstance()->findGame(client1_sd);

	//close sockets - we don't know who is which, so close using socket despcriptors from GameInfo
	close(g.getClientA());
	close(g.getClientB());

	//remove from list - by client 1 (arbitrary, it doens't matter)
	GamesInfoLists::getInstance()->removeGame(client1_sd);

	cout << "close command, line " << __LINE__ << "\tsender: " << client1_sd << "\n";

	//kill thread of game - to end game
	pthread_exit(NULL);
}
