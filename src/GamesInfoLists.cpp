#include "../include/GamesInfoLists.h"

#include <algorithm>

GamesInfoLists *GamesInfoLists::instance_ = 0;
pthread_mutex_t GamesInfoLists::lock_;

GamesInfoLists *GamesInfoLists::getInstance()
{
	if (instance_ == 0) {
		pthread_mutex_lock(&lock_);
		if (instance_ == 0) {
			instance_ = new GamesInfoLists();
		}
		pthread_mutex_unlock(&lock_);
	}
	return instance_;
}

//games_ vector will be initialized via default c'tor
GamesInfoLists::GamesInfoLists(): nullGame_("", -2) {}


GamesInfoLists::~GamesInfoLists() {
	//release GameInfos
	for (vector<GameInfo*>::iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//delete games Info
		delete *iter;
	}

}

void GamesInfoLists::resetInstance() {
	delete instance_; // REM : it works even if the pointer is NULL (does nothing then)
	instance_ = NULL; // so GetInstance will still work.
}


GameInfo* GamesInfoLists::findGame(int client_sd) {
	//search played games
	for (vector<GameInfo*>::iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if given client is one of the playing clients
		if ((*iter)->getClientA() == client_sd || (*iter)->getClientB() == client_sd) {
			//return game
			return *iter;
		}
	}

	//else - return null game
	return &nullGame_;
}


vector<GameInfo*>::iterator GamesInfoLists::findGamePosition(int client_sd) {
	//search played games
	for (vector<GameInfo*>::iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if given client is one of the playing clients
		if ((*iter)->getClientA() == client_sd || (*iter)->getClientB() == client_sd) {
			//return iter
			return iter;
		}
	}

	//else - return end of list
	return games_.end();
}

GameInfo* GamesInfoLists::findGame(string& name) {
	//search played games
	for (vector<GameInfo*>::iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if we found the given game
		if ((*iter)->getGameName() == name) {
			//return game info
			return (*iter);
		}
	}

	//else - return null game
	return &nullGame_;
}


void GamesInfoLists::removeGame(GameInfo* g) {
	//find game's position in vector
	vector<GameInfo*>::iterator pos = find(games_.begin(), games_.end(), g);

	//delete GameInfo pointer
	delete *pos;

	//lock - this is a common resource, we must protect
	pthread_mutex_lock(&vectorMutex_);

	//remove GameInfo
	games_.erase(pos);

	pthread_mutex_unlock(&vectorMutex_); //unlock
}


void GamesInfoLists::removeGame(int client_sd) {
	//find game's position in vector
	vector<GameInfo*>::iterator pos = findGamePosition(client_sd);

	//delete GameInfo pointer
	delete *pos;

	//lock - this is a common resource, we must protect
	pthread_mutex_lock(&vectorMutex_);

	//remove GameInfo
	games_.erase(pos);

	pthread_mutex_unlock(&vectorMutex_); //unlock
}


string GamesInfoLists::listWaitingGames() {
	string list = "";
	for (vector<GameInfo*>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if this is a waiting game
		if ((*iter)->isWaiting()) {
			//add game name to list with space between
			list = list + (*iter)->getGameName() + " ";
		}
	}
	return list;
}


int GamesInfoLists::startNewGame(string& name, int clientA) {
	//check if a game with the given name exists
	GameInfo* g = findGame(name);

	//if game is not null - a game with the given name exists, return 1
	if (*g != nullGame_) {
		return 1;
	}

	//else - create new game and add game to list
	GameInfo* game = new GameInfo(name, clientA);

	//lock - this is a common resource, we must protect
	pthread_mutex_lock(&vectorMutex_);
	games_.push_back(game);

	pthread_mutex_unlock(&vectorMutex_);

	//method ended successfully - return 0
	return 0;
}


GameInfo* GamesInfoLists::joinGame(string& name, int clientB, pthread_t& tid) {
	cout << "GameInfoLists," << "\tname: " << name << ", clientB: " << clientB  << ", tid: " << tid << "\tline " << __LINE__ << "\n"; //TODO
	//find game
	GameInfo* g = findGame(name);

	cout << "GameInfoLists, find game " << "\tline " << __LINE__ << "\n"; //TODO

	//if game is not null and is waiting - join it
	if (*g != nullGame_ && g->isWaiting()) {
		cout << "GameInfoLists, game not yet played " << "\tline " << __LINE__ << "\n"; //TODO
		g->play(clientB, tid);
		cout << "GameInfoLists, game played " << "\tline " << __LINE__ << "\n"; //TODO
		return g;
	}

	//else - game does not exist or is being played - return null game
	return &nullGame_;
}


vector<int> GamesInfoLists::getAllOpenSockets() {
	vector<int> sockets;

	for (vector<GameInfo*>::const_iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//push first client into vector
		sockets.push_back((*iter)->getClientA());

		//if game is not waiting (=game is playing) - add also second client (second client is also an open socket)
		if (!(*iter)->isWaiting()) {
			sockets.push_back((*iter)->getClientB());
		}
	}

	return sockets;
}

vector<pthread_t> GamesInfoLists::getAllThreadIDs() {
	vector<pthread_t> threads;

	for (vector<GameInfo*>::iterator iter = games_.begin(); iter != games_.end(); iter++) {
		//if game is not waiting (=game is playing) - add thread's id
		if (!(*iter)->isWaiting()) {
			threads.push_back((*iter)->getTID());
		}
	}

	return threads;
}
