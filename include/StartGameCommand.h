#ifndef STARTGAMECOMMAND_H_
#define STARTGAMECOMMAND_H_

#include "Command.h"

#include <vector>
#include <string>
using namespace std;

class StartGameCommand: public Command {
public:
	StartGameCommand(Server& s, GamesInfoLists& list);

	/**
		 * Executes the action of this command.
		 * In this case - makes given player (second argument in vector) start a new game with the
		 * given game name (first argument in vector).
		 *
		 * Calls GamesInfoLists to start a new game in Waiting status
		 *
		 * @param args arguments for command's execution, by the set protocol of the certain instance
		 */
	virtual void execute(vector<string> args);
};

#endif /* STARTMATCHCOMMAND_H_ */
