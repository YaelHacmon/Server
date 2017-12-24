/*
 * JoinCommand.cpp
 *
 *      Author: yael
 */

#include "JoinCommand.h"


using namespace std;

JoinCommand::JoinCommand(SetCommand &info): info(info) {}

JoinCommand::~JoinCommand() {}

void JoinCommand::execute(int sender, vector<string> args) {
	//TODO
	//info.""(sender, args);
}
