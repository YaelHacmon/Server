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


private:
	int port;
	int serverSocket;


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
	void handleGame(Game& g);

	/**
	 * Helper function for cleaner code: converts integer to string
	 */
	string toString(int a);

};



#endif /* SERVER_H_ */
