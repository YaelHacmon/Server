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
	enum MatchStatus {Waiting, Playing};

	//TODO - needed
	GameInfo();

	/**
	 * C'tor taking the game name and the first client
	 */
	GameInfo(std::string name, int clientA);

	/**
	 * Getter for client 1
	 */
	int getClientA() const;

	/**
	 * Getter for client 2
	 */
	int getClientB() const;

	/**
	 * Getter for game name
	 */
	const std::string& getGameName() const;

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

	bool interrupt; //TODO - remove?

	pthread_mutex_t statusMutex_;
	pthread_mutex_t interruptMutex_; //TODO - remove?

	/**
	 * Setter for client 1
	 */
	void setClientA(int clientA);

	/**
	 * Setter for client 2
	 */
	void setClientB(int clientB);

	/**
	 *  Setter for game status
	 */
	void setStatus(GameInfo::MatchStatus status);
};

#endif /* GAMEINFO_H_ */
