#include "../include/CommandManager.h"
#include "../include/PlayCommand.h"

using namespace std;

//c'tor - new commands should be added here
//TODO - see if objects can be on stack... probably not, due to scope or something
CommandsManager::CommandsManager() {
	commandsMap["play"] = new PlayCommand();
	// Add more commands...
}


CommandsManager::~CommandsManager() {
	//deletes map, using iterator
	map<string, Command *>::iterator it;
	for (it = commandsMap.begin(); it !=
			commandsMap.end(); it++) {
		delete it->second;
	}
}


void CommandsManager::executeCommand(string command, vector<string> args) {
	//get the correct command from map
	Command *commandObj = commandsMap[command];
	//execute the command
	commandObj->execute(args);
}




