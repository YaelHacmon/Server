#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "GamesInfoLists.h"
#include "Server.h"
#include "Command.h"
#include <map>

class CommandsManager {
public:
	/**
	 * Default c'tor, creates map with Commands.
	 * 	Since both CommunicationManager and GamesInfoLists are singleton pattern designs,
	 * no parameters are needed.
	 */
	CommandsManager();

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
