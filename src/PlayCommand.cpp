/*
 * PlayCommand.cpp
 */
#include "../include/PlayCommand.h"

void PlayCommand::execute(std::vector<std::string> args) {
	//get the 2 file descriptors
	int client1_sd = atoi(args[0].c_str());
	int client2_sd = atoi(args[1].c_str());

	//play the move - if a problem occured, close the game
	if (server_.playMove(client1_sd, client2_sd) == -1) {
		server_.closeGame(server_.findGame(client1_sd));
	}
	//else - command has been successfully executed
}

