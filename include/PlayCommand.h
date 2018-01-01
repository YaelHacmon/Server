#ifndef PLAYCOMMAND_H_
#define PLAYCOMMAND_H_

#include "Command.h"
using namespace std;

/**
 * Reresents a command notifying of one player making a moved
 */
class PlayCommand: public Command {
public:
	/**
	 * Constructor taking the server to be executed on, by reference.
	 * @param server to apply command to
	 */
	PlayCommand();

	/**
	 * Executes the action of this command.
	 * In this case - communicates one turn of the playing player (first argument in vector) to the other player (second argument).
	 *
	 * Reads the played move as integers then writes to other player (also as integers)
	 *
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 * @param tid id of current thread
	 */
	virtual void execute(std::vector<std::string>& args, pthread_t& tid);

protected:
	/**
	 * Closes game played now - calls server to close sockets and calls gameInfoLists to remove game
	 */
	virtual void closeGame(int client1_sd, int client2_sd);

	/**
	 * Reads integer from client1. (Protocol: -2 is noMoves flag)
	 * @return number read if succeeded, -1 if not
	 */
	virtual int readNum(int client1_sd, int client2_sd);

	/**
	 * Writes integer to client2.
	 * @return 1 if succeeded, 0 if not
	 */
	virtual int writeNum(int num, int client1_sd, int client2_sd);
};


#endif /* PLAYCOMMAND_H_ */
