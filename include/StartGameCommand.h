#ifndef STARTGAMECOMMAND_H_
#define STARTGAMECOMMAND_H_

#include "Command.h"

#include <vector>
#include <string>
using namespace std;

class StartGameCommand: public Command {
public:
	StartGameCommand();

	/**
	 * Executes the action of this command.
	 * In this case - makes given player (second argument in vector) start a new game with the
	 * given game name (first argument in vector).
	 *
	 * Calls GamesInfoLists to start a new game in Waiting status
	 *
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 */
	virtual void execute(std::vector<std::string>& args);

protected:
	/**
	 * Writes integer to client.
	 * @return 1 if succeeded, 0 if not
	 */
	virtual int writeNum(int num, int client_sd);
};

#endif /* STARTMATCHCOMMAND_H_ */
