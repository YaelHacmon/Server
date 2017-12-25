/*
 * ListGamesCommand.h
 *      Author: yael
 */

#ifndef LISTGAMESCOMMAND_H_
#define LISTGAMESCOMMAND_H_

#include "Command.h"
#include "Utilities.h"

#include <vector>
#include <string>
using namespace std;

class ListGamesCommand: public Command {
public:
	ListGamesCommand(Server &server);
	virtual ~ListGamesCommand();

	virtual void execute(vector<string> args);
};

#endif /* LISTGAMESCOMMAND_H_ */
