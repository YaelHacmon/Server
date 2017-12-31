/*
 * CommunicationManager.h
 * Integrates between the commands and the server.
 * Singleton pattern for easy access from commands.
 *
 * Most methods are delegation methods for server, to allow commands' actions
 */

#ifndef COMMUNICATIONMANAGER_H_
#define COMMUNICATIONMANAGER_H_

#pragma once

#include "GameInfo.h"
#include <string>

//temporary links to actual classes used
class Server;
class ClientHandler;
class CommandsManager;


class CommunicationManager
{
public:
    static CommunicationManager *getInstance();

	// static destructor for avoiding infinite loop when releasing real instance of class
	static void resetInstance();

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
	std::string readString(int client_sd);

	/**
	 * Reads string from client1.
	 * @return string read if succeeded, an empty string if not
	 */
	std::string readString(int client1_sd, int client2_sd);

	/**
	 * Writes string to client.
	 * @return 1 if succeeded, 0 if not
	 */
	int writeString(std::string& s, int client_sd);

	/**
	 * Return server of this CommunicationManager's instance
	 */
	Server& getServer();

	/**
	 * Beautifier method for closing both sockets and killing thread
	 */
	void exitThread(int client1_sd, int client2_sd);

	/**
	 * Handles the g client of this handler, using the given server.
	 * @param client_sd socket descriptor of client
	 */
	void handleClient(int client_sd);

	/**
	 * Handles the full game logic between two clients.
	 */
	void handleGame(GameInfo& g);

private:
	CommunicationManager(); // Private c'tor
	CommunicationManager(const CommunicationManager &other); // Private copy c'tor
	~CommunicationManager();

    static CommunicationManager *instance_;

    //server to call when asked to read\write
    Server* server_;

    //Client handler to call when asked to handle a client
    ClientHandler* handler_;
};



#endif /* COMMUNICATIONMANAGER_H_ */
