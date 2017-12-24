/*
 * StartMatchCommand.cpp
 *
 *  Created on: Dec 24, 2017
 *      Author: yael
 */

#include "../include/StartGameCommand.h"


using namespace std;

StartMatchCommand::StartMatchCommand(SetCommand &info): info(info) {}

StartMatchCommand::~StartMatchCommand() {}

void StartMatchCommand::execute(int sender, vector<string> args) {
	info.startNewMatch(sender, args);
}
