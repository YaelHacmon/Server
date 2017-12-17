/*
 * CommandManager.h
 */

#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "Command.h"
#include <map>
using namespace std;

class CommandsManager {
public:
	/**
	 * Default c'tor, creates map with Commands
	 */
	CommandsManager();

	/**
	 * Destructor to release map
	 */
	~CommandsManager();

	/**
	 * Executes a command by the given key and arguments
	 */
	void executeCommand(std::string command, std::vector<std::string> args);

private:
	//map of commands, where the key is a string (and value is the Command, of course)
	std::map<std::string, Command*> commandsMap;
};


#endif /* COMMANDMANAGER_H_ */
