/*
 * CloseCommand.h
 *
 *      Author: yael
 */

#ifndef CLOSECOMMAND_H_
#define CLOSECOMMAND_H_

#include "Command.h"
#include "SetCommand.h"

#include <vector>
#include <string>

class CloseCommand: public Command {
public:
	CloseCommand(SetCommand& info);
	virtual ~CloseCommand();

	virtual void execute(int sender, std::vector<std::string> args);

private:
	SetCommand& info;
};

#endif /* CLOSECOMMAND_H_ */
