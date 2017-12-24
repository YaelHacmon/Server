/*
 * SetCommand.cpp
 *
 *      Author: yael
 */

#include "SetCommand.h"
#include <iostream>
using namespace std;

SetCommand::SetCommand()
{
	lastCommand = SetCommand::split;
	lastCommandResult = -1;
}


SetCommand::~SetCommand()
{
	for (vector<GameInfo*>::iterator it = matches.begin(); it != matches.end(); ++it) {
		delete *it;
	}
}

vector<string> SetCommand::spliting(string& line) {
	istringstream iss(line);
	vector<string> args((istream_iterator<string>(iss)), istream_iterator<string>());
	return args;
}

//TODO?
void playMove(int client1_sd, string noMove)
{

}



//TODO - this is the old code (what was inside the big while loop - should now be inside playMove
int SetCommand::playMove(int client1_sd, int client2_sd) {
	//read row - first number sent
	int row = readNum(client1_sd, client2_sd);
	//if problem occurred - return from function
	if (row == -1) {
		return -1;
	}

	//if game is over (-2) - return from function TODO - required??
	if(row == -2){
		return -1;
	}

	//write row to other player
	//if an error occurred - return from function
	if(!writeNum(row, client1_sd, client2_sd)) {
		return -1;
	}

	//if other player made a move (did not send -1) - read and write column of player's move
	if (row != -1) {
		//read column
		int column = readNum(client1_sd, client2_sd);
		//if problem occurred - return from function
		if (column == -1) {
			return -1;
		}

		//write column
		//if an error occurred - return from function
		if(!writeNum(column, client1_sd, client2_sd)) {
			return -1;
		}
	}

	//method ended successfully - return 0 (game is not over)
	return 0;
}



int SetCommand::readNum(int client1_sd, int client2_sd) {
	int num;
	//read number sent
	int n = read(client1_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error reading row from socket" << endl;
		return -1;
	}else if (n == 0) {
		//if no bytes were read from client - client1 has disconnected
		//notify other client (client2) - write (-3) to opponent
		num = -3;
		int n = write(client2_sd, &num, sizeof(num));

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing row to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return -1;
	}

	//otherwise - all is well, return read row
	return num;
}


int SetCommand::writeNum(int num, int client1_sd, int client2_sd) {
	//write number to opponent
	int n = write(client2_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error writing row to socket" << endl;
		return 0;

	} else if (n == 0) {
		//if no bytes were written to client - client2 has disconnected
		//notify other client (client1) - write (-3) to opponent
		num = -3;
		int n = write(client1_sd, &num, sizeof(num));

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing row to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return 0;
	}

	//all went well - return 1
	return 1;
}


string& SetCommand::findGame(int client_sd) {
	//search played games
	for (vector<Game>::const_iterator iter = playingGames_.begin(); iter != playingGames_.end(); iter++) {
		//if given client is one of the playing clients
		if (iter->client1_sd == client_sd || iter->client2_sd == client_sd) {
			//return game name
			return iter->name;
		}
	}

	//search waiting games
	for (vector<Game>::const_iterator iter = waitingGames_.begin(); iter != waitingGames_.end(); iter++) {
		//if given client is the waiting client
		if (iter->client1_sd == client_sd) {
			//return game name
			return iter->name;
		}
	}

	//else - return the empty string
	return "";
}

bool SetCommand::sendMessageToClient(int client, string& msg) {
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
void SetCommand::startNewMatch(int client1_sd, vector<string> args) {
	lastCommand = SetCommand::Start;

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


SetCommand::CommandOption getLastCommand() const
{

}

int getLastCommandResult() const
{

}

