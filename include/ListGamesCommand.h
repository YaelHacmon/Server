#ifndef LISTGAMESCOMMAND_H_
#define LISTGAMESCOMMAND_H_

#include "Command.h"

#include <vector>
#include <string>
using namespace std;

class ListGamesCommand: public Command {
public:
	ListGamesCommand(Server& s, GamesInfoLists& list);

	/**
	 * Executes the action of this command.
	 * In this case - sends given player (first argument in vector) the list of waiting games.
	 *
	 * Gets string with list of waiting games from GamesInfoLists, then sends to given client.
	 *
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 */
	virtual void execute(vector<string> args);
};

#endif /* LISTGAMESCOMMAND_H_ */
