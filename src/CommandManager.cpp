#include "../include/CommandManager.h"
#include "../include/PlayCommand.h"
#include "../include/StartGameCommand.h"
#include "../include/ListGamesCommand.h"
#include "../include/JoinCommand.h"
#include "../include/CloseCommand.h"
#include "../include/Server.h"
#include "../include/GamesInfoLists.h"

using namespace std;

//c'tor - new commands should be added here
CommandsManager::CommandsManager(Server& s, GamesInfoLists& list) {
	commandsMap["play"] = new PlayCommand(s, list);
	commandsMap["start"] = new StartGameCommand(s, list);
	commandsMap["list_games"] = new ListGamesCommand(s, list);
	commandsMap["join"] = new JoinCommand(s, list);
	commandsMap["close"] = new CloseCommand(s, list);
}

CommandsManager::~CommandsManager() {
	//delete all commands
	for (map<string, Command *>::iterator it = commandsMap.begin(); it != commandsMap.end(); it++) {
		delete it->second;
	}
}

void CommandsManager::executeCommand(string command, vector<string> args) {
	Command *currCommand = commandsMap[command];
	currCommand->execute(args);
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




