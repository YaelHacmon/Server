#include "../include/GamesInfoLists.h"

//for find()
#include <algorithm>

//games_ will be initialized via default c'tor
GamesInfoLists::GamesInfoLists() {}

GamesInfoLists::~GamesInfoLists() {
	for (vector<GameInfo>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		delete *iter; //TODO - okay?
	}
}


string& GamesInfoLists::findGame(int client_sd) {
	//search played games
	for (vector<GameInfo>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if given client is one of the playing clients
		if (iter->getClientA() == client_sd || iter->getClientB() == client_sd) {
			//return game name
			return iter->getGameName();
		}
	}

	//else - return the empty string
	return "";
}


GameInfo& GamesInfoLists::findGame(string& name) {
	//search played games
	for (vector<GameInfo>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if we found the given game
		if (iter->getGameName() == name) {
			//return game info
			return *iter;
		}
	}

	//else - return null
	return NULL;
}

/**
 * Closes the given game (game given by name): searches for it in lists and removes from playing lists and closes descriptors.
 */
void GamesInfoLists::closeGame(GameInfo& g) {
	//TODO - does this need a mutex lock, or is the fact we are using a built in vector enough? and is it small enough??
	pthread_mutex_lock(&vectorMutex_);
	//find game's position in vector
	vector<GameInfo>::iterator pos = find(games_.begin(), games_.end(), g);
	//remove game
	games_.erase(pos);
	pthread_mutex_unlock(&vectorMutex_);
}

/**
 * Returns the list of games waiting for another player, separated with a space between them
 */
string GamesInfoLists::listWaitingGames() {
	string list = "";
	for (vector<GameInfo>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if this is a waiting game
		if (iter->isWaiting()) {
			//add game name to list with space between
			list = list + iter->getGameName() + " ";
		}
	}
	return list;
}

/**
 * Starts a new game in the waiting list, with the given name and client's socket descriptor. Given sd is of first (black) player.
 */
//TODO - why args??
void startNewMatch(int client1_sd, vector<string> args);

//TODO?
bool sendMessageToClient(int client, string& msg);


void GamesInfoLists::startNewGame(string name, int clientA) {
	GameInfo g = new GameInfo(name, clientA);
	games_.push_back(g); //TODO - does this need a mutex?
}


GameInfo GamesInfoLists::joinGame(string name, int clientB) {
	//find game
	GameInfo g = findGame(name);
	//if game is not null - join it
	if (g != NULL) {
		g.play(clientB);
		return g;
	}

	return NULL;
}
