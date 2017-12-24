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

	GameInfo();
	~GameInfo();

	/**
	 * Finds and returns the name of the game played by the given file descriptor.
	 * Helper method for closing a game in case of an error in playMove().
	 * String can be returned by reference as it is not created inside method.
	 *
	 * In case of game not found - the empty string is returned
	 */
	std::string& findGame(int client_sd);


	int getClientA() const;
	void setClientA(int clientA);

	int getClientB() const;
	void setClientB(int clientB);

	const std::string& getGameName() const;
	void setGameName(const std::string& gameName);

	GameInfo::MatchStatus getStatus();
	void setStatus(GameInfo::MatchStatus status);

	bool clientInMatch(int client) const;

	int getOtherClient(int firstClient) const;

	pthread_mutex_t getStatusMutex() const;

	void setInterrupt(bool interrupt);

private:
	std::string gameName;

	int clientA;
	int clientB;

	GameInfo::MatchStatus status;

	bool interrupt;

	pthread_mutex_t statusMutex;
	pthread_mutex_t interruptMutex;
};

#endif /* GAMEINFO_H_ */
