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

	/**
	 * Handles one turn of the clients
	 */
	void handleClients(int client1_sd, int client2_sd);

	/**
	 * Reads from client1.
	 * @return number read if succeeded, -1 if not (and should break loop)
	 */
	int readNum(int client1_sd, int client2_sd);

	/**
	 * Reads from client1.
	 * @return 1 if succeeded, 0 if not (and should break loop)
	 */
	int writeNum(int num, int client1_sd, int client2_sd);
};



#endif /* SERVER_H_ */
