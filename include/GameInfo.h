/*
 * GameInfo.h
 *      Author: yael
 */

#ifndef GAMEINFO_H_
#define GAMEINFO_H_

#include <string>

class GameInfo {
public:
	enum MatchStatus {Waiting, Playing, Over};

	GameInfo();

	/**
	 * C'tor taking the game name and the first client
	 */
	GameInfo(std::string name, int clientA);

	~GameInfo();

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
	 * Checks if game is over. Returns true if is over, false otherwise
	 */
	bool isOver();

	/**
	 * Sets status to "over"
	 */
	void gameEnded();

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

	GameInfo::MatchStatus status_;

	pthread_mutex_t statusMutex_;
	pthread_mutex_t clientAMutex_;
	pthread_mutex_t clientBMutex_;
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
