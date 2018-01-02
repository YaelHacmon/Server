#include "../include/StartGameCommand.h"
#include "../include/GamesInfoLists.h"
#include <cstdlib> //for atoi()
#include <unistd.h> //for close(), read(), write()

using namespace std;

StartGameCommand::StartGameCommand() {}

void StartGameCommand::execute(vector<string>& args, pthread_t& tid) {
	//get given game name (first argument) and client's sd (second argument)
	string name = args[0];
	int clientA = atoi(args[1].c_str());

	//call list to start new game. if a game with the given name exists, 1 will be returned. 0 if method was successful
	//if starting a new game was not successful
	if (GamesInfoLists::getInstance()->startNewGame(name, clientA)) {
		//try to write -1 (error code) to client - notify client that initialization is impossible
		writeNum(-1, clientA); //returned value does not matter to us (if client disconnected - his problem :) )

	} else if (!writeNum(0, clientA)) {
		//otherwise - send 0 to client, to show initialization worked
		//if writing failed, client disconnected - remove the game just created
		GamesInfoLists::getInstance()->removeGame(clientA);
	}
}


int StartGameCommand::writeNum(int num, int client_sd) {
	//write number to client
	int n = write(client_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error writing number to socket" << endl;
		return 0;

	} else if (n == 0) {
		//if no bytes were written to client - client has disconnected
		//closing clients will happen when returning with a error

		return 0;
	}

	//all went well - return 1
	return 1;
}
