/*
 * GameInfo.h
 *      Author: yael
 */

#ifndef GAMEINFO_H_
#define GAMEINFO_H_

#include <string>

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

	/**
	 * Getter for thread's id. Non-const to allow usage
	 */
	pthread_t& getTID();

	/**
	 * Checks if game is waiting. Returns true if is waiting, false otherwise
	 */
	bool isWaiting() const;

	/**
	 * Play game with given second client, in given thread.
	 * If game is not in waiting status - nothing is done
	 */
	void play(int clientB, pthread_t id);

	/**
	 * Swaps the two clients, to use when playing the turns in game
	 */
	void swapClients();

	/**
	 * Overloading of == for checking if two games are equal
	 */
	bool operator ==(const GameInfo &g) const;

	/**
	 * Overloading of != for checking if two games are not equal
	 */
	bool operator !=(const GameInfo &g) const;


private:
	std::string gameName_;

	int clientA_;
	int clientB_;

	//thread id of game
	pthread_t tid_;

	GameInfo::MatchStatus status_;

	pthread_mutex_t statusMutex_;
	pthread_mutex_t clientAMutex_;
	pthread_mutex_t clientBMutex_;
	pthread_mutex_t tidMutex_;

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

	/**
	 * Setter for thread's id
	 */
	void setTID(pthread_t& id);
};

#endif /* GAMEINFO_H_ */
