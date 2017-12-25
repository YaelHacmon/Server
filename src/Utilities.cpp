/*
 * Utilities.cpp
 *
 *      Author: yael
 */

#include "../include/Utilities.h"
#include <iostream>
using namespace std;

Utilities::Utilities()
{
	lastCommand = Utilities::split;
	lastCommandResult = -1;
}


Utilities::~Utilities()
{
	for (vector<GameInfo*>::iterator it = matches.begin(); it != matches.end(); ++it) {
		delete *it;
	}
}

vector<string> Utilities::split(string& line) {
	istringstream iss(line);
	vector<string> args((istream_iterator<string>(iss)), istream_iterator<string>());
	return args;
}

//TODO?
void playMove(int client1_sd, string noMove)
{

}



//TODO - this is the old code (what was inside the big while loop - should now be inside playMove
int Utilities::playMove(int client1_sd, int client2_sd) {

}


string& Utilities::findGame(int client_sd) {
//TODO - extracted to GamesInfoLists
}

bool Utilities::sendMessageToClient(int client, string& msg) {
	int writeSize;
	writeSize = send(client, msg.c_str(), msg.length(), SEND_FLAGS);
	if (writeSize == -1) {
		//Error in writing
		cout << "Error writing to client " << client << endl;
		return false;
	}
	return true;
}
//TODO - ON THE FACE
void Utilities::startNewMatch(int client1_sd, vector<string> args) {
	lastCommand = Utilities::Start;

	GameInfo *newGame;

	pthread_mutex_lock(&matchesMutex);
	for (vector<GameInfo *>::iterator it = matches.begin(); it != matches.end(); ++it) {
		if ((*it)->getGameName() == args[0]) {
			pthread_mutex_lock(&stringsMutex);
			pthread_mutex_unlock(&stringsMutex);
			pthread_mutex_unlock(&matchesMutex);
			return;
		}
	}
	pthread_mutex_unlock(&matchesMutex);

	newGame = new GameInfo();

	newGame->setClientA(client1_sd);
	newGame->setGameName(args[0]);
	newGame->setStatus(GameInfo::Waiting);

	pthread_mutex_lock(&matchesMutex);
	matches.push_back(newGame);
	pthread_mutex_unlock(&matchesMutex);

	pthread_mutex_lock(&clientMapMutex);
	matchClientMap[clientASocket] = newGame;
	pthread_mutex_unlock(&clientMapMutex);

	pthread_mutex_lock(&stringsMutex);
	pthread_mutex_unlock(&stringsMutex);
}


void closeGame(string& name)
{

}

string listGames()
{

}


void joinGame(int client2_sd,vector<string> args)
{

}


Utilities::CommandOption getLastCommand() const
{

}

int getLastCommandResult() const
{

}

