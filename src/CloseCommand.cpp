/*
 * CloseCommand.cpp
 *      Author: yael
 */

#include "CloseCommand.h"

using namespace std;

CloseCommand::CloseCommand(SetCommand &info): info(info) {}

CloseCommand::~CloseCommand() {}

void CloseCommand::execute(int sender, vector<string> args) {
	//TODO
	//info.""(sender, args);
}
