/*
 * GameInfo.h
 *      Author: yael
 */

#ifndef GAMEINFO_H_
#define GAMEINFO_H_

#include <string>
#include <pthread.h>

class GameInfo {
public:
	enum MatchStatus {ReadyForMatch, Waiting, Playing, Ending};

	//TODO - needed
	GameInfo();

	/**
	 * C'tor taking the game name and the first client
	 */
	GameInfo(std::string name, int clientA);

	//TODO - needed?
	~GameInfo();

	/**
	 * Finds and returns the name of the game played by the given file descriptor.
	 * Helper method for closing a game in case of an error in playMove().
	 * String can be returned by reference as it is not created inside method.
	 *
	 * In case of game not found - the empty string is returned
	 */
	//TODO - why?
	std::string& findGame(int client_sd) const;


	/**
	 * Getter for client 1
	 */
	int getClientA() const;
	/**
	 * Setter for client 1
	 */
	//TODO - should be private?
	void setClientA(int clientA);

	/**
	 * Getter for client 2
	 */
	int getClientB() const;
	/**
	 * Setter for client 2
	 */
	//TODO - should be private?
	void setClientB(int clientB);

	/**
	 * Getter for game name
	 */
	const std::string& getGameName() const;
	/**
	 * Setter for game name
	 */
	//TODO - should be private?
	void setGameName(const std::string& gameName);

	/**
	 * Getter+setter for game status
	 */
	//TODO - should be private?
	GameInfo::MatchStatus getStatus();
	void setStatus(GameInfo::MatchStatus status);

	//TODO - what for?
	bool clientInMatch(int client) const;

	//TODO - ?
	int getOtherClient(int firstClient) const;

	//TODO - why needed?
	pthread_mutex_t getStatusMutex() const;

	//TODO - what does this mean?
	void setInterrupt(bool interrupt);

	/**
	 * Checks if game is waiting. Returns true if is waiting, false otherwise
	 */
	bool isWaiting() const;

	/**
	 * Play game with given second client.
	 * If game is not in waiting status - nothing is done
	 */
	void play(int clientB);

	/**
	 * Swaps the two clients, to use when playing the turns in game
	 */
	void swapClients();

	/**
	 * Overloading of == for using "find()" method from <algorithm>
	 */
	bool operator ==(const GameInfo &g) const;

private:
	std::string gameName_;

	int clientA_;
	int clientB_;

	GameInfo::MatchStatus status_;

	bool interrupt;

	pthread_mutex_t statusMutex_;
	pthread_mutex_t interruptMutex_;
};

#endif /* GAMEINFO_H_ */
