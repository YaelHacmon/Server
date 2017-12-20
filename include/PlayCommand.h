#ifndef PLAYCOMMAND_H_
#define PLAYCOMMAND_H_

#include "Command.h"

/**
 * Reresents a command notifying of one player making a moved
 */
class PlayCommand: public Command {
public:
	/**
	 * Constructor taking the server to be executed on, by reference.
	 * @param server to apply command to
	 */
	PlayCommand(Server s): Command(s) {};

	/**
	 * Executes the action of this command.
	 * In this case - send the other player the played move. (In case of no move - sends -1,-1)
	 *
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 */
	virtual void execute(std::vector<std::string> args) = 0;
};


#endif /* PLAYCOMMAND_H_ */
