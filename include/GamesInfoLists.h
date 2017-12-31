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
 * Singleton design to allow easy access from all commands
 */
class GamesInfoLists {
public:
	static GamesInfoLists *getInstance();

	// static destructor for avoiding infinite loop when releasing real instance of class
	static void resetInstance();

	//d'tor
	~GamesInfoLists();

	/**
	 * Finds and returns the name of the game played by the given file descriptor.
	 * Helper method for closing a game in case of an error in playMove().
	 * String can be returned by reference as it is not created inside method.
	 *
	 * In case of game not found - a null game (clientA=-2, name=empty string) is returned
	 */
	GameInfo& findGame(int client_sd);

	/**
	 * Finds and returns the GameInfo of the game with the given name.
	 * GameInfo can be returned by reference as it is not created inside method.
	 *
	 * In case of game not found - a null game (clientA=-2, name=empty string) is returned
	 */
	GameInfo& findGame(string& name);

	/**
	 * Removes the given game (game given by name): searches for it in lists and removes from playing lists
	 */
	void removeGame(GameInfo& g);

	/**
	 * Removes the game played by given client: searches for it in lists and removes from playing lists
	 */
	void removeGame(int client_sd);

	/**
	 * Returns the list of games waiting for another player, separated with a space between them
	 * If non exist - an empty string is returned
	 */
	string listWaitingGames();

	/**
	 * Starts a new waiting game in list, with the given name and client's socket descriptor. Given sd is of first (black) player.
	 * @return 0 if succeeded, 1 if a game with the given name exists already.
	 */
	int startNewGame(string name, int clientA);

	/**
	 * Joins given player (by sd) to a given existing game (by name).
	 * Checks that game exists and can be joined.
	 * @return the updated GameInfo, or a null game (clientA=-2, name=empty string)
	 * if either no such game exists or given game is already being played
	 */
	GameInfo joinGame(string name, int clientB);

	/**
	 * Returns the socket descriptors of all currently open sockets, in a vector of integers.
	 * Used for closing the server
	 */
	vector<int> getAllOpenSockets();

private:
	GamesInfoLists() {}; // Private c'tor
	GamesInfoLists(const GamesInfoLists &other); // Private copy c'tor

	//static class member
	static GamesInfoLists *instance_;

	//list of the games waiting to be played or being played
	std::vector<GameInfo> games_;

	//mutex for locking in code acting on vector
	pthread_mutex_t vectorMutex_;

	//null game - to avoid recreating each time
	GameInfo nullGame_;

	/**
	 * Finds and returns the iterator of the game with the given client.
	 *
	 * In case of game not found - end() is returned
	 */
	vector<GameInfo>::iterator findGamePosition(int client_sd);
};

#endif /* SRC_Utilities_H_ */



#endif /* GAMESINFOS_H_ */
