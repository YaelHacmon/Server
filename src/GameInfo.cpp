/*
 * GameInfo.cpp
 *
 *  Created on: Dec 23, 2017
 *      Author: yael
 */
#include "GameInfo.h"

//TODO
GameInfo::GameInfo(std::string name, int clientA_): clientA_(clientA_), gameName_(name), clientB_(-1),
						status_(GameInfo::Waiting), interrupt(false) {};

GameInfo::GameInfo(): clientA_(-1), clientB_(-1),
					  gameName_(""), status_(GameInfo::ReadyForMatch),
					  interrupt(false) {
	//Nothing right now

}

int GameInfo::getClientA() const {
	return clientA_;
}

void GameInfo::setClientA(int clientA) {
	this->clientA_ = clientA;
}

int GameInfo::getClientB() const {
	return clientB_;
}

void GameInfo::setClientB(int clientB) {
	this->clientB_ = clientB;
}

const std::string& GameInfo::getGameName() const {
	return gameName_;
}

void GameInfo::setGameName(const std::string& gameName) {
	this->gameName_ = gameName;
}

GameInfo::MatchStatus GameInfo::getStatus() {
	pthread_mutex_lock(&statusMutex);
	GameInfo::MatchStatus toReturn = status_;
	pthread_mutex_unlock(&statusMutex_);
	return toReturn;
}

void GameInfo::setStatus(GameInfo::MatchStatus status) {
	pthread_mutex_lock(&statusMutex_);
	this->status_ = status;
	pthread_mutex_unlock(&statusMutex_);
}

bool GameInfo::clientInMatch(int client) const {
	return ((clientA_ == client) || (clientB_ == client));
}

int GameInfo::getOtherClient(int firstClient) const {
	if (firstClient == clientA_) {
		return clientB_;
	} else if (firstClient == clientB_) {
		return clientA_;
	}
	return -1;
}

pthread_mutex_t GameInfo::getStatusMutex() const {
	return statusMutex_;
}

void GameInfo::setInterrupt(bool interrupt) {
	this->interrupt = interrupt;
}


bool GameInfo::isWaiting() const {
	return status_ == MatchStatus::Waiting;
}

void GameInfo::play(int clientB) {
	//update the second client
	setClientB(clientB);
	//update the status
	setStatus(MatchStatus::Playing);
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
