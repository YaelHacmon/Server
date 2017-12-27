#include "../include/GamesInfoLists.h"

//for find()
#include <algorithm>

//games_ will be initialized via default c'tor
GamesInfoLists::GamesInfoLists() {}

GamesInfoLists::~GamesInfoLists() {
	for (vector<GameInfo>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		delete *iter;
	}
}


GameInfo& GamesInfoLists::findGame(int client_sd) {
	//search played games
	for (vector<GameInfo>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if given client is one of the playing clients
		if (iter->getClientA() == client_sd || iter->getClientB() == client_sd) {
			//return game name
			return *iter;
		}
	}

	//else - return NULL
	return NULL;
}


vector<GameInfo>::iterator GamesInfoLists::findGamePosition(int client_sd) {
	//search played games
	for (vector<GameInfo>::iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if given client is one of the playing clients
		if (iter->getClientA() == client_sd || iter->getClientB() == client_sd) {
			//return iter
			return iter;
		}
	}

	//else - return end of list
	return games_.end();
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
void GamesInfoLists::removeGame(GameInfo& g) {
	//find game's position in vector
	vector<GameInfo>::iterator pos = find(games_.begin(), games_.end(), g);

	//lock - this is a common resource, we must protect
	pthread_mutex_lock(&vectorMutex_);

	//delete GameInfo
	delete *pos; //TODO - inside lock? will work?
	//remove game
	games_.erase(pos);

	pthread_mutex_unlock(&vectorMutex_);
}


void GamesInfoLists::removeGame(int client_sd) {
	//find game's position in vector
	vector<GameInfo>::iterator pos = findGame(client_sd);

	//lock - this is a common resource, we must protect
	pthread_mutex_lock(&vectorMutex_);

	//delete GameInfo
	delete *pos; //TODO - inside lock? will work?
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


void GamesInfoLists::startNewGame(string name, int clientA) {
	//create new game
	GameInfo g = new GameInfo(name, clientA);

	//lock - this is a common resource, we must protect
	pthread_mutex_lock(&vectorMutex_);
	//add game to list
	games_.push_back(g);
	pthread_mutex_unlock(&vectorMutex_);
}


GameInfo GamesInfoLists::joinGame(string name, int clientB) {
	//find game
	GameInfo g = findGame(name);
	//if game is not null and is waiting - join it
	if (g != NULL && g.isWaiting()) {
		g.play(clientB);
		return g;
	}

	//else - game does not exist or is being played - return null
	return NULL;
}


vector<int> GamesInfoLists::getAllOpenSockets() {
	vector<int> sockets;

	for (vector<GameInfo>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//push first client into vector
		sockets.push_back(iter->getClientA());

		//if game is not waiting (=game is playing) - add also second client (second client is also an open socket)
		if (!iter->isWaiting()) {
			sockets.push_back(iter->getClientB());
		}
	}
}
