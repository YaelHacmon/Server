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
#include "CommandsManager.h"

using namespace std;

/*
 * This server handles connecting two clients to one
 * another
 */
class Server {
public:
	/*
	 * Getting the port from file
	 */
	Server(string fileName);

	void start();

	/**
	 * Reads integer from client1. (Protocol: -2 is noMoves flag)
	 * @return number read if succeeded, -1 if not
	 */
	int readNum(int client1_sd, int client2_sd);

	/**
	 * Writes integer to client2.
	 * @return 1 if succeeded, 0 if not
	 */
	int writeNum(int num, int client1_sd, int client2_sd);

	/**
	 * Writes integer to client.
	 * @return 1 if succeeded, 0 if not
	 */
	int writeNum(int num, int client_sd);

	/**
	 * Reads string from client.
	 * @return string read if succeeded, an empty string if not
	 */
	string readString(int client_sd);

	/**
	 * Reads string from client1.
	 * @return string read if succeeded, an empty string if not
	 */
	string readString(int client1_sd, int client2_sd);

	/**
	 * Writes string to client.
	 * @return 1 if succeeded, 0 if not
	 */
	int writeString(string s, int client_sd);


	/**
	 * Getter for server socket, for use from static functions
	 */
	int getServerSocket();

	/**
	 * Getter for commands manager, for use from static functions
	 */
	vector<pthread_t> getThreadVector();

	/**
	 * Getter for commands manager, for use from static functions
	 */
	CommandsManager getCommandManager();

private:
	int port;
	int serverSocket;

	//vector of threads
	vector<pthread_t> threads_;

	//manager of commands
	CommandsManager commandManager_;

	//handles the lists of existing games and their information
	GamesInfoLists gameList_;

	/**
	 * Endless loop for accepting clients in separate thread.
	 * Function must be static to be passed to pthread_create()
	 * @param s server to be used in accepting the clients
	 */
	static void* acceptClients(void* s);

	/**
	 * Handles the initial communication with a client: asking to start\join a game and accepting answers
	 * Function must be static to be passed to pthread_create()
	 * @param cd client handler that can handle the given client
	 */
	static void* handleSingleClient(void* ch);

	/**
	 * Handles the full game between two clients.
	 */
	void handleGame(GameInfo& g);

	/**
	 * Beautifier method for closing both sockets and killing thread
	 */
	void exitThread(int client1_sd, int client2_sd);
};



#endif /* SERVER_H_ */
