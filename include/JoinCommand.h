/*
 * JoinCommand.h
 *      Author: yael
 */


#ifndef JOINCOMMAND_H_
#define JOINCOMMAND_H_

#include "Command.h"
#include "SetCommand.h"

#include <vector>
#include <string>
using namespace std;

class JoinCommand: public Command {
public:
	JoinCommand(SetCommand& info);
	virtual ~JoinCommand();

	virtual void execute(int sender, vector<string> args);

private:
	SetCommand& info;
};

#endif /* JOINCOMMAND_H_ */
