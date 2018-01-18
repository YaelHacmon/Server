#include "../include/ClientHandler.h"
#include "../include/Utility.h"
#include <unistd.h> //for close(), read(), write()
#include <iostream>

//set maximal command length to 60 bytes (longest command - start/close <name> - is at most 56 characters)
#define MAX_COMMAND_LENGTH 60

using namespace std;

ClientHandler::ClientHandler() {}

void ClientHandler::handleClient(int client_sd) {
	//read string of command from given client
	string command = readString(client_sd);

	//if a problem occurred - close socket and terminate thread
	if(command == "") {
		//close socket (if socket is already closed - does nothing)
		close(client_sd);

		//exit method -> exits thread
		return;
	}

	//split command by space
	vector<string> args = Utility::split(command);

	//make command the first word
	command = args[0];

	//remove command from arguments
	args.erase(args.begin());

	//add the client's file descriptor as the last arguments
	args.insert(args.end(), Utility::toString(client_sd)); //add to arguments

	//execute command
	CommandsManager::getInstance()->executeCommand(command, args);
	//explanation - will either open a game in the game lists, and exit thread, or create new thread to play a game (via join)
}


string ClientHandler::readString(int client_sd) {
	char str[MAX_COMMAND_LENGTH];

	//read string sent
	int n = read(client_sd, str, MAX_COMMAND_LENGTH);

	if (n == -1) {
		std::cout << "Error reading string from socket" << std::endl;
		return "";
	}else if (n == 0) {
		//if no bytes were read from client - client has disconnected, return NULL (an error occurred)
		//closing clients will happen when returning with a error
		return "";
	}

	//otherwise - all is well, return string read row
	return string(str);
}
