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
#include "ThreadPool.h"

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

private:
	int port;
	int serverSocket;
	pthread_t serverThreadId; //id of acceptClients's thread
	ThreadPool pool_; //thread pool of threads

	//handler of client - static to make a class member, instead of instance memberWWWWWW
	static ClientHandler* handler_;

	//static method for getting handler
	static ClientHandler& getHandler();

	/**
	 * Getter for server socket, for use from static functions
	 */
	int getServerSocket();

	/**
	 * Getter for thread pool, for use from static functions
	 */
	ThreadPool& getThreadPool();

	/**
	 * Endless loop for accepting clients in separate thread.
	 * Function must be static to be passed to pthread_create()
	 * @param s - server to accept through
	 */
	static void *acceptClients(void *s);

	/**
	 * Handles the initial communication with a client: asking to start\join a game and accepting answers
	 * Function must be static to be passed to pthread_create()
	 * @param client_socket socket of client to handle
	 */
	static void* handleSingleClient(void* client_socket);

	/**
	 * A static wrapper that calls executeTasks()
	 */
	static void *execute(void *arg);
};



#endif /* SERVER_H_ */
