#include "../include/ClientHandler.h"
#include "../include/CommunicationManager.h"

using namespace std;

ClientHandler::ClientHandler(CommandsManager& cmd): commandManager_(cmd) {}

void ClientHandler::handleClient(int client_sd) {
	//read string of command from given client
	string command = CommunicationManager::getInstance()->readString(client_sd);

	//if a problem occurred - close socket and terminate thread
	if(command == "") {
		//close socket (if socket is already closed - does nothing)
		close(client_sd);

		//exit method -> exits thread
		return;
	}

	//split command by space
	vector<string> args = split(command);

	//make command the first word
	command = args[0];
	//remove command from arguments
	args.erase(args.begin());

	//add the client's file descriptor as the last arguments
	args.insert(args.end(), toString(client_sd)); //add to arguments

	//execute command
	commandManager_.executeCommand(command, args);

	//explanation - will either open a game in the game lists, and exit thread, or create new thread to play a game (via join)
}


void ClientHandler::handleGame(GameInfo& g) {
	//Sending 1 to show him he was the first to enter
	int color = 1;
	int n = write(g.getClientA(), &color, sizeof(color));
	if (n == -1) {
		//if problem occured - close sockets and kill thread
		CommunicationManager::getInstance()->getServer().exitThread(g.getClientA(), g.getClientB());
	}

	//Sending 2 to client 2 to show him he was the second to enter
	color = 2;
	n = write(g.getClientB(), &color, sizeof(color));
	if (n == -1) {
		//if problem occured - close sockets and kill thread
		CommunicationManager::getInstance()->exitThread(g.getClientA(), g.getClientB());
	}

	//play game - accept commands from players
	//loop will keep going until thread is terminated
	while (true) {
		string command = CommunicationManager::getInstance()->readString(g.getClientA(), g.getClientB());

		//if a problem occurred - close socket and terminate thread (other client has already been notified of disconnection)
		if(command == "") {
			CommunicationManager::getInstance()->exitThread(g.getClientA(), g.getClientB());
		}

		//split command by space
		vector<string> args = split(command);

		//make command the first word
		command = args[0];
		//remove command from arguments
		args.erase(args.begin());

		//add the two file descriptors as the last two arguments
		//sd of client 1
		args.insert(args.end(), toString(g.getClientA()));

		//sd of client 2
		args.insert(args.end(), toString(g.getClientB())); //add to arguments

		//execute command
		commandManager_.executeCommand(command, args);

		//switch players and keep communicating game
		g.swapClients();

	}//end of game loop - will end via killing thread ("close" command)
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
