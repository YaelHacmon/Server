/*
 * GameInfo.cpp
 *
 *  Created on: Dec 23, 2017
 *      Author: yael
 */
#include "GameInfo.h"

GameInfo::GameInfo(): clientA(-1), clientB(-1),
					  gameName(""), status(GameInfo::ReadyForMatch),
					  interrupt(false) {
	//Nothing right now

}

int GameInfo::getClientA() const {
	return clientA;
}

void GameInfo::setClientA(int clientA) {
	this->clientA = clientA;
}

int GameInfo::getClientB() const {
	return clientB;
}

void GameInfo::setClientB(int clientB) {
	this->clientB = clientB;
}

const std::string& GameInfo::getGameName() const {
	return gameName;
}

void GameInfo::setGameName(const std::string& gameName) {
	this->gameName = gameName;
}

GameInfo::MatchStatus GameInfo::getStatus() {
	pthread_mutex_lock(&statusMutex);
	GameInfo::MatchStatus toReturn = status;
	pthread_mutex_unlock(&statusMutex);
	return toReturn;
}

void GameInfo::setStatus(GameInfo::MatchStatus status) {
	pthread_mutex_lock(&statusMutex);
	this->status = status;
	pthread_mutex_unlock(&statusMutex);
}

bool GameInfo::clientInMatch(int client) const {
	return ((clientA == client) || (clientB == client));
}

int GameInfo::getOtherClient(int firstClient) const {
	if (firstClient == clientA) {
		return clientB;
	} else if (firstClient == clientB) {
		return clientA;
	}
	return -1;
}

pthread_mutex_t GameInfo::getStatusMutex() const {
	return statusMutex;
}

void GameInfo::setInterrupt(bool interrupt) {
	this->interrupt = interrupt;
}
