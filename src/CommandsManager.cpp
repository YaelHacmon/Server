#include "../include/CommandsManager.h"
#include "../include/PlayCommand.h"
#include "../include/StartGameCommand.h"
#include "../include/ListGamesCommand.h"
#include "../include/JoinCommand.h"
#include "../include/CloseCommand.h"
#include "../include/Server.h"
#include "../include/GamesInfoLists.h"

using namespace std;

//c'tor - new commands should be added here
CommandsManager::CommandsManager() {
	commandsMap["play"] = new PlayCommand();
	commandsMap["start"] = new StartGameCommand();
	commandsMap["list_games"] = new ListGamesCommand();
	commandsMap["join"] = new JoinCommand();
	commandsMap["close"] = new CloseCommand();
}

CommandsManager::~CommandsManager() {
	//delete all commands
	for (map<string, Command *>::iterator it = commandsMap.begin(); it != commandsMap.end(); it++) {
		delete it->second;
	}
}

void CommandsManager::executeCommand(string command, vector<string> args) {
	//get the correct command from map
	Command *commandObj = commandsMap[command];
	//execute the command
	commandObj->execute(args);
}




