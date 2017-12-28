#include "../include/ClientHandler.h"

using namespace std;

ClientHandler::ClientHandler(Server& s, int client_sd): server_(s), client_(client_sd) {}

void ClientHandler::handle() {
	//read string of command from given client
	string command = server_.readString(client_);

	//if a problem occurred - close socket and terminate thread
	if(command == "") {
		//close socket (if socket is already closed - does nothing)
		close(client_sd);

		//exit thread
		pthread_exit(NULL); //TODO - should switch to return?
	}

	//split command by space
	vector<string> args = split(command);

	//make command the first word
	command = args[0];
	//remove command from arguments
	args.erase(args.begin());

	//add the client's file descriptor as the last arguments
	args.insert(args.end(), toString(client_)); //add to arguments

	//execute command
	server_.getCommandManager().executeCommand(command, args);

	//explanation - will either open a game in the game lists, and exit thread, or create new thread to play a game (via join)

}


vector<string> ClientHandler::split(string& line) {
	istringstream iss(line);
	vector<string> args((istream_iterator<string>(iss)), istream_iterator<string>());
	return args;
}


string ClientHandler::toString(int a) {
	//declare stringstream
	stringstream ss;
	//add integer to stream
	ss << a;
	//make stream to string
	string str = ss.str();
	//return string
	return str;
}
