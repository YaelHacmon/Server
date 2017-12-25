/*
 * StartGameCommand.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: yael
 */

#include "../include/StartGameCommand.h"


using namespace std;

StartGameCommand::StartGameCommand(Server &server): info(info) {}

StartGameCommand::~StartGameCommand() {}

void StartGameCommand::execute(int sender, vector<string> args) {
	info.startNewMatch(sender, args);
}
