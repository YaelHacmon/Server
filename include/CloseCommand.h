/*
 * CloseCommand.h
 *
 *      Author: yael
 */

#ifndef CLOSECOMMAND_H_
#define CLOSECOMMAND_H_

#include "Command.h"
#include "Utilities.h"

#include <vector>
#include <string>

class CloseCommand: public Command {
public:
	CloseCommand(Server &server);
	virtual ~CloseCommand();

	virtual void execute(std::vector<std::string> args);
};

#endif /* CLOSECOMMAND_H_ */
