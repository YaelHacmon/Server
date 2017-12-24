/*
 * ListGamesCommand.h
 *      Author: yael
 */

#ifndef LISTGAMESCOMMAND_H_
#define LISTGAMESCOMMAND_H_

#include "Command.h"
#include "SetCommand.h"

#include <vector>
#include <string>
using namespace std;

class ListGamesCommand: public Command {
public:
	ListGamesCommand(SetCommand& info);
	virtual ~ListGamesCommand();

	virtual void execute(int sender, vector<string> args);

private:
	SetCommand& info;
};

#endif /* LISTGAMESCOMMAND_H_ */
