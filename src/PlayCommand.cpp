/*
 * PlayCommand.cpp
 */
#include "../include/PlayCommand.h"


using namespace std;

PlayCommand::PlayCommand(Server& s): server_(s) {}

PlayCommand::~PlayCommand() {} //TODO - is this needed? because there is no dynamic memory used...

void PlayCommand::execute(vector<string> args) {
	//get clients - first and second argument, by
	int client1_sd = atoi(args[0].c_str());
	int client2_sd = atoi(args[1].c_str());

	//read row - first number sent
	int row = server_.readNum(client1_sd, client2_sd);
	//if problem occurred - close game
	if (row == -1) {
		//close using client1 (arbitrarily)
		server_.closeGame(client1_sd);
	}

	//if game is over (-2) -  close game TODO - required??
	if(row == -2){
		//close using client1 (arbitrarily)
		server_.closeGame(client1_sd);
	}

	//write row to other player
	//if an error occurred -  close game
	if(!server_.writeNum(row, client1_sd, client2_sd)) {
		//close using client1 (arbitrarily)
		server_.closeGame(client1_sd);
	}

	//if other player made a move (did not send -1) - read and write column of player's move
	if (row != -1) {
		//read column
		int column = server_.readNum(client1_sd, client2_sd);
		//if problem occurred - close game
		if (column == -1) {
			//close using client1 (arbitrarily)
			server_.closeGame(client1_sd);
		}

		//write column
		//if an error occurred - close game
		if(!server_.writeNum(column, client1_sd, client2_sd)) {
			//close using client1 (arbitrarily)
			server_.closeGame(client1_sd);
		}
	}


	//method ended successfully - return 0 (game is not over) TODO - what??
}

