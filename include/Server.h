/*
 * Server.h
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

#include "GamesInfoLists.h"

using namespace std;

class Server {
	/*
	 * This server handles connecting two clients to one
	 * another
	 */

public:
	Server(int port);
	/*
	 * Getting the port from file
	 */
	Server(string fileName);
	void start();
	void stop();

	/**
	 * Reads from client1.
	 * @return number read if succeeded, -1 if not (and should break loop and close game)
	 */
	int readNum(int client1_sd, int client2_sd);

	/**
	 * Reads from client1.
	 * @return 1 if succeeded, 0 if not (and should break loop and close game)
	 */
	int writeNum(int num, int client1_sd, int client2_sd);

	/**
	 * Closes the game played by the given client.
	 * Helper method for aborting when an error occurs
	 *
	 * @param socket descriptor of client
	 */
	void closeGame(int client_sd);

	/**
	 * Join the game at the given index in list, joining player's socket descriptor is given. Joining player is always second (white).
	 */
	void joinGame(int client2_sd, string name);

private:
	int port;
	int serverSocket;

	//manager of commands
	CommandsManager commandManager_;

	//handles the lists of existing games and their information
	GamesInfoLists gameList_;

	/**TODO
	 * Handles the game of two clients
	 */
	//void handleClients(int client1_sd, int client2_sd);

	/**
	 * Handles the initial communication with a client: asking to start\join a game and accepting answers
	 */
	void handleSingleClient(int client_sd);

	/**
	 * Handles the full game between two clients.
	 */
	void handleGame(GameInfo& g);

	/**
	 * Helper function for cleaner code: converts integer to string
	 */
	string toString(int a);

};



#endif /* SERVER_H_ */
