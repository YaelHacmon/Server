#include "../include/ClientHandler.h"
#include "../include/Utility.h"
#include <unistd.h> //for close(), read(), write()
#include <iostream>

//set maximal command length to 60 bytes (longest command - start/close <name> - is at most 56 characters)
#define MAX_COMMAND_LENGTH 60

using namespace std;

ClientHandler::ClientHandler() {}

void ClientHandler::handleClient(int client_sd, pthread_t tid) {
	cout << "\t\tin ClientHandler: handle client\n"; //TODO

	//read string of command from given client
	string command = readString(client_sd);

	cout << "\t\t\tXXin ClientHandler: after read" << command << "\n"; //TODO
	cout << "\t\t\tpassed line" << __LINE__ << "\n";

	//if a problem occurred - close socket and terminate thread
	if(command == "") {
		//close socket (if socket is already closed - does nothing)
	cout << "\t\t\tpassed line" << __LINE__ << "\n";
		close(client_sd);

	cout << "\t\t\tpassed line" << __LINE__ << "\n";
		//exit method -> exits thread
		return;
	cout << "\t\t\tpassed line" << __LINE__ << "\n";
	}

	cout << "\t\t\tpassed line" << __LINE__ << "\n";
	//split command by space
	vector<string> args = Utility::split(command);

	cout << "\t\t\tpassed line" << __LINE__ << "\n";
	//make command the first word
	command = args[0];
	cout << "\t\t\tpassed line" << __LINE__ << "\n";
	//remove command from arguments
	args.erase(args.begin());

	cout << "\t\t\tpassed" << __LINE__ << "\n";
	//add the client's file descriptor as the last arguments
	args.insert(args.end(), Utility::toString(client_sd)); //add to arguments

	cout << "\t\t\tpassed line" << __LINE__ << "\n";
	//execute command
	CommandsManager::getInstance()->executeCommand(command, args, tid);

	cout << "\t\t\tpassed line" << __LINE__ << "\n";
	//explanation - will either open a game in the game lists, and exit thread, or create new thread to play a game (via join)
}


string ClientHandler::readString(int client_sd) {
	cout << "\t\t\tin ClientHandler: read string\n"; //TODO

	char str[MAX_COMMAND_LENGTH];

	//read string sent
	int n = read(client_sd, str, MAX_COMMAND_LENGTH);

	cout << (n==-1) << endl;

	if (n == -1) {
		std::cout << "Error reading string from socket" << std::endl;
		return "";
	}else if (n == 0) {
		//if no bytes were read from client - client has disconnected, return NULL (an error occurred)
		//closing clients will happen when returning with a error
		cout << "n=0\n"; //TODO
		return "";
	}

	cout << "\t\t\tin ClientHandler: after read - " << str << "\n"; //TODO

	//otherwise - all is well, return string read row
	return string(str);
}
