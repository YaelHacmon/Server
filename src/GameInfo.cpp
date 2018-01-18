/*
 * GameInfo.cpp
 */
#include "GameInfo.h"

GameInfo::GameInfo(std::string name, int clientA_): gameName_(name), clientA_(clientA_), clientB_(-1), status_(GameInfo::Waiting) {
	pthread_mutex_init(&clientAMutex_, NULL);
	pthread_mutex_init(&clientBMutex_, NULL);
	pthread_mutex_init(&statusMutex_, NULL);
};

GameInfo::~GameInfo() {
	pthread_mutex_destroy(&clientAMutex_);
	pthread_mutex_destroy(&clientBMutex_);
	pthread_mutex_destroy(&statusMutex_);
}

int GameInfo::getClientA() const {
	return clientA_;
}

void GameInfo::setClientA(int clientA) {
	pthread_mutex_lock(&clientAMutex_);
	this->clientA_ = clientA;
	pthread_mutex_unlock(&clientAMutex_);
}

int GameInfo::getClientB() const {
	return clientB_;
}

void GameInfo::setClientB(int clientB) {
	pthread_mutex_lock(&clientBMutex_);
	this->clientB_ = clientB;
	pthread_mutex_unlock(&clientBMutex_);
}

const std::string& GameInfo::getGameName() const {
	return gameName_;
}

void GameInfo::setStatus(GameInfo::MatchStatus status) {
	pthread_mutex_lock(&statusMutex_);
	status_ = status;
	pthread_mutex_unlock(&statusMutex_);
}

bool GameInfo::isWaiting() const {
	return status_ == GameInfo::Waiting;
}


void GameInfo::play(int clientB) {
	//update the second client
	setClientB(clientB);
	//update the status
	setStatus(GameInfo::Playing);
}


bool GameInfo::isOver() {
	return status_ == GameInfo::Over;
}


void GameInfo::gameEnded() {
	setStatus(GameInfo::Over);
}


void GameInfo::swapClients() {
	//save client A
	int temp = getClientA();
	//make client A be client B
	setClientA(getClientB());
	//make client B be temp=client A
	setClientB(temp);
}


bool GameInfo::operator ==(const GameInfo &g) const {
	//two games are equal if they have the same name
	return (g.gameName_==gameName_);
}

bool GameInfo::operator !=(const GameInfo &g) const {
	//two games are not equal if they have a different name
	return (g.gameName_!=gameName_);
}
