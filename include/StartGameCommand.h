/*
 * StartGameCommand.h
 *      Author: yael
 */

#ifndef STARTGAMECOMMAND_H_
#define STARTGAMECOMMAND_H_

#include "Command.h"
#include "Utilities.h"

#include <vector>
#include <string>
using namespace std;

class StartGameCommand: public Command {
public:
	StartGameCommand(Server &server);
	virtual ~StartGameCommand();

	virtual void execute(vector<string> args);
};

#endif /* STARTMATCHCOMMAND_H_ */
