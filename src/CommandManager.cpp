#include "../include/CommandManager.h"
#include "../include/PlayCommand.h"

using namespace std;

//c'tor - new commands should be added here
//TODO - see if objects can be on stack... probably not, due to scope or something
CommandsManager::CommandsManager(Utilities& s) {
	commandsMap["play"] = new PlayCommand(s);
	commandsMap["start"] = new StartMatchCommand(s);
	commandsMap["list_games"] = new ListGamesCommand(s);
	commandsMap["join"] = new JoinCommand(s);
	commandsMap["close"] = new CloseCommand(s);
}

}

CommandsManager::~CommandsManager() {
	delete commandsMap["start"];
	delete commandsMap["list_games"];
	delete commandsMap["join"];
	delete commandsMap["play"];
	delete commandsMap["close"];
}

void CommandsManager::executeCommand(int sender, string command, vector<string> args) {
	Command *currCommand = commandsMap[command];
	currCommand->execute(sender, args);
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




