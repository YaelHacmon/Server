#ifndef JOINCOMMAND_H_
#define JOINCOMMAND_H_

#include "../include/Command.h"
#include "../include/GameInfo.h"


#include <vector>
#include <string>
using namespace std;

class JoinCommand: public Command {
public:
	JoinCommand();

	/**
	 * Executes the action of this command.
	 * In this case - makes given player (second argument in vector) join the given game name (first argument in vector).
	 *
	 * Updates the game's status and the second client, then calls the method of playing the game
	 *
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 * @param tid id of current thread
	 */
	virtual void execute(vector<string>& args, pthread_t& tid);

protected:
	/**
	 * Handles the full game logic between two clients.
	 */
	virtual void handleGame(GameInfo& g);

	/**
	 * Reads string from client1.
	 * @return string read if succeeded, an empty string if not
	 */
	virtual string readString(int client1_sd, int client2_sd);

	/**
	 * Beautifier method for closing both sockets and killing thread
	 */
	virtual void exitThread(int client1_sd, int client2_sd);
};

#endif /* JOINCOMMAND_H_ */
