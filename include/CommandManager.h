#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "Command.h"
#include "Server.h"
#include <map>
using namespace std;

class CommandsManager {
public:
	/**
	 * Default c'tor, creates map with Commands. Accepts server for commands to act on
	 */
	CommandsManager(Server& s, GamesInfoLists& list);

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
	map<string, Command*> commandsMap;

};


#endif /* COMMANDMANAGER_H_ */
