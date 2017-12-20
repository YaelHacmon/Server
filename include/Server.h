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

	/**
	 * Closes the given game (game given by name): searches for it in lists and removes from playing lists and closes descriptors.
	 */
	void closeGame(string& name);

	/**
	 * Returns the list of games waiting for another player, separated with a space between them
	 */
	string listGames();

	/**
	 * Starts a new game in the waiting list, with the given name and client's socket descriptor. Given sd is of first (black) player.
	 */
	void startGame(string& name, int client1_sd);

	/**
	 * Join the game at the given index in list, joining player's socket descriptor is given. Joining player is always second (white).
	 */
	void joinGame(int index, int client2_sd);

	/**
	 * Reads a move from the given client 1 and sends it to the given client 2. Moves are read and sent as integers.
	 * Returns 0 if game is not over and all read/write actions succeeded, -1 otherwise (if game should be closed)
	 */
	int playMove(int client1_sd, int client2_sd);

	/**
	 * Finds and returns the name of the game played by the given file descriptor.
	 * Helper method for closing a game in case of an error in playMove().
	 * String can be returned by reference as it is not created inside method.
	 *
	 * In case of game not found - the empty string is returned
	 */
	string& findGame(int client_sd);

private:
	int port;
	int serverSocket;

	/**
	 * Struct with game information: two client socket descriptors and the game name
	 */
	struct Game {
		std::string name;
		int client1_sd;
		int client2_sd;
	};

	//lists of the games waiting to be played or being played
	std::vector<Game> waitingGames_;
	std::vector<Game> playingGames_;

	//handler of commands
	CommandsManager commandManager_;

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
	 * Helper function for cleaner code: converts integer to string
	 */
	string toString(int a);

	/**
	 * Helper function for splitting a command by space
	 */
	vector<string> split(string& line);
};



#endif /* SERVER_H_ */
