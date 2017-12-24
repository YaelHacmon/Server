/*
 * StartGameCommand.h
 *      Author: yael
 */

#ifndef STARTGAMECOMMAND_H_
#define STARTGAMECOMMAND_H_

#include "Command.h"
#include "SetCommand.h"

#include <vector>
#include <string>
using namespace std;

class StartGameCommand: public Command {
public:
	StartGameCommand(SetCommand& info);
	virtual ~StartGameCommand();

	virtual void execute(int sender,vector<string> args);

private:
	SetCommand& info;
};

#endif /* STARTMATCHCOMMAND_H_ */
