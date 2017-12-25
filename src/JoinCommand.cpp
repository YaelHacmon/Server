/*
 * JoinCommand.cpp
 *
 *      Author: yael
 */

#include "JoinCommand.h"


using namespace std;

JoinCommand::JoinCommand(Server &server): server_(server) {}

JoinCommand::~JoinCommand() {}

void JoinCommand::execute(vector<string> args) {
	//ask server to join given game (first argument), by name and with client's sd (second argument)
	server_.joinGame(atoi(args[1].c_str()), args[0]);
}
