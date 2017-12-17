/*
 * CommandManager.h
 */

#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include <map>
#include "Command.h"

class CommandsManager {
public:
	CommandsManager();
	~CommandsManager();
	void executeCommand(std::string command, std::vector<std::string> args);
private:
	std::map<std::string, Command*> commandsMap;
};


#endif /* COMMANDMANAGER_H_ */
