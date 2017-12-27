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
	PlayCommand(Server& s, GamesInfoLists& list);

	/**
	 * Executes the action of this command.
	 * In this case - communicates one turn of the playing player (first argument in vector) to the other player (second argument).
	 *
	 * Reads the played move as integers then writes to other player (also as integers)
	 *
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 */
	virtual void execute(vector<string> args);

protected:
	/**
	 * Closes game played now - calls server to close sockets and calls gameInfoLists to remove game
	 */
	virtual void closeGame(int client1_sd, int client2_sd);
};


#endif /* PLAYCOMMAND_H_ */
