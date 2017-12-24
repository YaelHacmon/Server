/*
 * PlayCommand.cpp
 */
#include "../include/PlayCommand.h"


using namespace std;

PlayCommand::PlayCommand(SetCommand &info): info(info) {}

PlayCommand::~PlayCommand() {}

void PlayCommand::execute(int sender, vector<string> args) {
	//TODO
}

