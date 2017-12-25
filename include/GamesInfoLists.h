#ifndef GAMESINFOS_H_
#define GAMESINFOS_H_

#ifndef SRC_Utilities_H_
#define SRC_Utilities_H_

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <string>

#include "GameInfo.h"

using namespace std;

/**
 * Class to hold and manage all the games played via the server.
 */
class GamesInfoLists {
public:
	//empty c'tor
	GamesInfoLists();

	//d'tor
	~GamesInfoLists();

	/**
	 * Finds and returns the name of the game played by the given file descriptor.
	 * Helper method for closing a game in case of an error in playMove().
	 * String can be returned by reference as it is not created inside method.
	 *
	 * In case of game not found - the empty string is returned
	 */
	string& findGame(int client_sd);

	/**
	 * Finds and returns the GameInfo of the game with the given name.
	 * GameInfo can be returned by reference as it is not created inside method.
	 *
	 * In case of game not found - null is returned
	 */
	GameInfo& findGame(string& name);

	/**
	 * Closes the given game (game given by name): searches for it in lists and removes from playing lists and closes descriptors.
	 */
	void closeGame(GameInfo& g);

	/**
	 * Returns the list of games waiting for another player, separated with a space between them
	 */
	string listWaitingGames();

	/**
	 * Starts a new game in the waiting list, with the given name and client's socket descriptor. Given sd is of first (black) player.
	 */
	//TODO - why args??
	void startNewMatch(int client1_sd, vector<string> args);

	//TODO?
	bool sendMessageToClient(int client, string& msg);

	/**
	 * Adds a new game to the list, as a waiting game
	 */
	void startNewGame(string name, int clientA);

	/**
	 * Joins given player (by sd) to a given existing game (by name).
	 * Returns the updated GameInfo, or NULL if no such game exists
	 */
	GameInfo joinGame(string name, int clientB);

private:
	//list of the games waiting to be played or being played
	std::vector<GameInfo> games_;

	//TODO - needed?
	pthread_mutex_t vectorMutex_;
};

#endif /* SRC_Utilities_H_ */



#endif /* GAMESINFOS_H_ */
