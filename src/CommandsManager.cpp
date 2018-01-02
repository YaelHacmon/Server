#include "../include/CommandsManager.h"
#include "../include/PlayCommand.h"
#include "../include/StartGameCommand.h"
#include "../include/ListGamesCommand.h"
#include "../include/JoinCommand.h"
#include "../include/CloseCommand.h"
#include "../include/GamesInfoLists.h"

using namespace std;

CommandsManager* CommandsManager::instance_ = 0;
pthread_mutex_t CommandsManager::lock_;

CommandsManager* CommandsManager::getInstance() {
	if (instance_ == 0) {
		pthread_mutex_lock(&lock_);
		if (instance_ == 0) {
			instance_ = new CommandsManager();
		}
		pthread_mutex_unlock(&lock_);
	}
	return instance_;
}


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

void CommandsManager::executeCommand(string& command, vector<string>& args, pthread_t& tid) {
	//get the correct command from map
	Command *commandObj = commandsMap[command];

	//execute the command
	commandObj->execute(args, tid);
}

void CommandsManager::resetInstance() {
	delete instance_; // REM : it works even if the pointer is NULL (does nothing then)
	instance_ = NULL; // so GetInstance will still work.
}
