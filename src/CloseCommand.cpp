#include "../include/CloseCommand.h"
#include "../include/GamesInfoLists.h"
#include <unistd.h> //for close()
#include <cstdlib> //for atoi()

using namespace std;

CloseCommand::CloseCommand() {}


void CloseCommand::execute(vector<string>& args) {
	//find game by client1's sd (arbitrary)
	GameInfo* g = GamesInfoLists::getInstance()->findGame(args[0]);

	//close sockets - we don't know who is which, so close using socket despcriptors from GameInfo
	close(g->getClientA());
	close(g->getClientB());

	//remove from list - by client 1 (arbitrary, it doens't matter)
	//game is marked as over by method
	GamesInfoLists::getInstance()->removeGame(g);

	//return from method to free thread
}
