#ifndef CLOSECOMMAND_H_
#define CLOSECOMMAND_H_

#include "Command.h"

#include <vector>
#include <string>

class CloseCommand: public Command {
public:
	CloseCommand();

	/**
	 * Executes the action of this command.
	 * In this case - closes the game (name in first argument) played by the two given players (second and third argument in vector)
	 *
	 * Changes game status to ENDING
	 *
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 */
	virtual void execute(std::vector<std::string>& args);
};

#endif /* CLOSECOMMAND_H_ */
