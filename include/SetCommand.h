/*
 * SetCommand.h
 *      Author: yael
 */

#ifndef SRC_SETCOMMAND_H_
#define SRC_SETCOMMAND_H_


#include "GameInfo.h"

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <string>

using namespace std;

class SetCommand {
public:
	SetCommand();
	virtual ~SetCommand();

	enum CommandOption {Start, Join, Play, Close, List};

	/**
	 * Helper function for splitting a command by space
	 */
	vector<string> split(string& line);

	string& findGame(int client_sd);

	/**
	 * Reads a move from the given client 1 and sends it to the given client 2. Moves are read and sent as integers.
	 * Returns 0 if game is not over and all read/write actions succeeded, -1 otherwise (if game should be closed)
	 */
	int playMove(int client1_sd, int client2_sd);

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

	//TODO?
	void playMove(int client1_sd, string noMove);

	/**
	 * Closes the given game (game given by name): searches for it in lists and removes from playing lists and closes descriptors.
	 */
	void closeGame(string& name);

	/**
	 * Returns the list of games waiting for another player, separated with a space between them
	 */
	string listGames();

	/**
	 * Join the game at the given index in list, joining player's socket descriptor is given. Joining player is always second (white).
	 */
	void joinGame(int client2_sd,vector<string> args);

	//TODO?
	SetCommand::CommandOption getLastCommand() const;

	//TODO?
	int getLastCommandResult() const;

	/**
	 * Starts a new game in the waiting list, with the given name and client's socket descriptor. Given sd is of first (black) player.
	 */
	void startNewMatch(int client1_sd, vector<string> args);

	//TODO?
	bool sendMessageToClient(int client, string& msg);

private:


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

	//GameInfo matches[5];
	vector<GameInfo*> matches;

	map<int, GameInfo*> matchClientMap;

	SetCommand::CommandOption lastCommand;

	int lastCommandResult;

};

#endif /* SRC_SETCOMMAND_H_ */
