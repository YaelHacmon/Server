#include "../include/CloseCommand.h"
#include "../include/GamesInfoLists.h"
#include <unistd.h> //for close()
#include <cstdlib> //for atoi()

using namespace std;

CloseCommand::CloseCommand() {}


void CloseCommand::execute(vector<string>& args, pthread_t& tid) {
	//find game by client1's sd (arbitrary)
	GameInfo* g = GamesInfoLists::getInstance()->findGame(args[0]);

	cout << "\t\t\tin CloseCommand: line " << __LINE__  << "\n"; //TODO

	//close sockets - we don't know who is which, so close using socket despcriptors from GameInfo
	close(g->getClientA());
	close(g->getClientB());

	//remove from list - by client 1 (arbitrary, it doens't matter)
	GamesInfoLists::getInstance()->removeGame(g);

	cout << "\t\t\tin CloseCommand, line " << __LINE__ << "\n";

	//kill thread of game - to end game
	pthread_exit(NULL);
}
