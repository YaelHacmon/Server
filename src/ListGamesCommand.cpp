#include "../include/ListGamesCommand.h"
#include "../include/GamesInfoLists.h"
#include <unistd.h> //for close()
#include <cstdlib> //for atoi()

//set maximal string length (for list_games command) to 1024 bytes (1 kb)
#define MAX_STRING_LENGTH 1024

using namespace std;

ListGamesCommand::ListGamesCommand() {}

void ListGamesCommand::execute(vector<string>& args, pthread_t& tid) {
	//get given client's sd (first argument)
	int client = atoi(args[0].c_str());

	//get string of waiting games
	string list = GamesInfoLists::getInstance()->listWaitingGames();

	//send list via server
	//if problem occured - close client (exiting is done right after, anyway)
	if (!writeString(list, client)) {
		close(client);
	}

	//end thread via return from method
}

int ListGamesCommand::writeString(string& s, int client_sd) {
	//convert to char array
	const char* str = s.c_str();

	//write number to opponent
	int n = write(client_sd, str, sizeof(str));
	if (n == -1) {
		cout << "Error writing string to socket" << endl;
		return 0;

	}else if (n == 0) {
		//if no bytes were read from client - client has disconnected, return 0 (an error occurred)
		//closing clients will happen when returning with a error
		return 0;
	}

	//all went well - return 1
	return 1;
}
