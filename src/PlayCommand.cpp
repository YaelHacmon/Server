#include "../include/PlayCommand.h"
#include "../include/GamesInfoLists.h"
#include <pthread.h>
#include <unistd.h> //for close(), read(), write()
#include <cstdlib> //for atoi()

using namespace std;

PlayCommand::PlayCommand() {}

void PlayCommand::execute(vector<string>& args) {
	//get clients - first and second argument, by
	int client1_sd = atoi(args[0].c_str());
	int client2_sd = atoi(args[1].c_str());

	//read row - first number sent
	int row = readNum(client1_sd, client2_sd);
	//if problem occurred - close game
	if (row == -1) {
		//close game
		closeGame(client1_sd, client2_sd);
	}

	//write row to other player, agreed flag for no moves is (-2) in notifying server, (-1) in receiving from server
	if (row == -2) {
		//if 'no moves' signal was sent - translate to (-1)
		row = -1;
	}

	//if an error occurred -  close game
	if(!writeNum(row, client1_sd, client2_sd)) {
		//close game
		closeGame(client1_sd, client2_sd);
	}

	//if other player made a move (row is not -1 now) - read and write column of player's move
	if (row != -1) {
		//read column
		int column = readNum(client1_sd, client2_sd);
		//if problem occurred - close game
		if (column == -1) {
			//close game
			closeGame(client1_sd, client2_sd);
		}

		//write column
		//if an error occurred - close game
		if(!writeNum(column, client1_sd, client2_sd)) {
			//close game
			closeGame(client1_sd, client2_sd);
		}
	}
}


void PlayCommand::closeGame(int client1_sd, int client2_sd) {
	//close sockets
	close(client1_sd);
	close(client2_sd);
	//remove from list - by client 1 (arbitrary, it doens't matter)
	GamesInfoLists::getInstance()->removeGame(client1_sd);

	//return from method - game status has changed to "over"
}

int PlayCommand::readNum(int client1_sd, int client2_sd) {
	int num;
	//read number sent
	int n = read(client1_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error reading number from socket" << endl;
		return -1;
	}else if (n == 0) {
		//if no bytes were read from client - client1 has disconnected
		//notify other client (client2) - write (-3) to opponent
		num = -3;
		int n = write(client2_sd, &num, sizeof(num));

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing number to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return -1;
	}

	//otherwise - all is well, return number read row
	return num;
}


int PlayCommand::writeNum(int num, int client1_sd, int client2_sd) {
	//write number to opponent
	int n = write(client2_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error writing number to socket" << endl;
		return 0;

	} else if (n == 0) {
		//if no bytes were written to client - client2 has disconnected
		//notify other client (client1) - write (-3) to opponent
		num = -3;
		int n = write(client1_sd, &num, sizeof(num));

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing number to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return 0;
	}

	//all went well - return 1
	return 1;
}
