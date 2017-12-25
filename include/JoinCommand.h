/*
 * JoinCommand.h
 *      Author: yael
 */


#ifndef JOINCOMMAND_H_
#define JOINCOMMAND_H_

#include "Command.h"
#include "Utilities.h"

#include <vector>
#include <string>
using namespace std;

class JoinCommand: public Command {
public:
	JoinCommand(Server &server);
	virtual ~JoinCommand();

	/**
	 * Executes the action of this command.
	 * In this case - makes given player (second argument in vector) join the given game name (first argument in vector).
	 *
	 * Updates the game's status and the second client, then calls the method of playing the game
	 *
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 */
	virtual void execute(vector<string> args);
};

#endif /* JOINCOMMAND_H_ */
