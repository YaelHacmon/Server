#include "../include/PlayCommand.h"
#include <pthread.h>
#include <unistd.h> //for close()
#include <cstdlib> //for atoi()

using namespace std;

PlayCommand::PlayCommand(Server& s, GamesInfoLists& list): Command(s, list) {}

void PlayCommand::execute(vector<string> args) {
	//get clients - first and second argument, by
	int client1_sd = atoi(args[0].c_str());
	int client2_sd = atoi(args[1].c_str());

	//read row - first number sent
	int row = server_.readNum(client1_sd, client2_sd);
	//if problem occurred - close game
	if (row == -1) {
		//close game
		closeGame(client1_sd, client2_sd);
	}

	//write row to other player, agreed flag for no moves is (-2)
	//if an error occurred -  close game
	if(!server_.writeNum(row, client1_sd, client2_sd)) {
		//close game
		closeGame(client1_sd, client2_sd);
	}

	//if other player made a move (did not send -2) - read and write column of player's move
	if (row != -2) {
		//read column
		int column = server_.readNum(client1_sd, client2_sd);
		//if problem occurred - close game
		if (column == -1) {
			//close game
			closeGame(client1_sd, client2_sd);
		}

		//write column
		//if an error occurred - close game
		if(!server_.writeNum(column, client1_sd, client2_sd)) {
			//close game
			closeGame(client1_sd, client2_sd);
		}
	}

	//method ended successfully
	//thread should not be killed (as opposed to other commands), because game is still being played
}


void PlayCommand::closeGame(int client1_sd, int client2_sd) {
	//close sockets
	close(client1_sd);
	close(client2_sd);
	//remove from list - by client 1 (arbitrary, it doens't matter)
	list_.removeGame(client1_sd);

	//kill thread - an error occured and we cannot keep on playing
	//(client will read 0 from server and know that server disconnected)
	pthread_exit(NULL);
}
