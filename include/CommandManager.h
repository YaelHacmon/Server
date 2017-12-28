#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "GamesInfoLists.h"
#include <map>
#include "Server.h"
#include "Command.h"

class CommandsManager {
public:
	/**
	 * Default c'tor, creates map with Commands.
	 * @param s server for commands to act on
	 * @param list of games to act on
	 * declare server as class to avoid header include loop
	 */
	CommandsManager(class Server& s, GamesInfoLists& list);

	/**
	 * Destructor to release map
	 */
	~CommandsManager();

	/**
	 * Executes a command by the given key and arguments
	 */
	void executeCommand(string command, vector<string> args);

private:
	//map of commands, where the key is a string (and value is the Command, of course)
	std::map<std::string, Command*> commandsMap;

};


#endif /* COMMANDMANAGER_H_ */
