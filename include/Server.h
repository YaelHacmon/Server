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
#include "ClientHandler.h"

using namespace std;

/*
 * This server handles connecting two clients to one
 * another
 */
class Server {
public:
	/*
	 * C'tor with filename for port number and client handler to handle client
	 * @param fileName name of file with port number
	 * @param ch client handler to use in handling client
	 */
	Server(const string& fileName, ClientHandler& ch);

	/**
	 * Starts the server - in a new thread
	 */
	void start();

	/**
	 * Stops the server - cancels all threads and exits.
	 */
	void stop();

	/**
	 * Getter for server socket, for use from static functions
	 */
	int getServerSocket();

private:
	int port;
	int serverSocket;
	pthread_t serverThreadId; //id of acceptClients's thread

	//handler of client - static to make a class member, instead of instance memberWWWWWW
	static ClientHandler* handler_;

	//static method for getting handler
	static ClientHandler& getHandler();

	/**
	 * Endless loop for accepting clients in separate thread.
	 * Function must be static to be passed to pthread_create()
	 * @param socket - socket of server to accept with
	 */
	static void *acceptClients(void *socket);

	/**
	 * Handles the initial communication with a client: asking to start\join a game and accepting answers
	 * Function must be static to be passed to pthread_create()
	 * @param info - information to use in handling client: socket of client to handle and thread's id
	 */
	static void* handleSingleClient(void* info);

	//struct for passing both thread id and socket to handleSingleClient() method
	struct ClientHandleInfo {
		int socket; //socket number
		pthread_t tid; //thread id of current thread
	};
};



#endif /* SERVER_H_ */
