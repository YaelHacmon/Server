#include "../include/JoinCommand.h"
#include "../include/GamesInfoLists.h"
#include "../include/CommandsManager.h"
#include "../include/Utility.h"
#include <cstdlib> //for atoi()
#include <unistd.h> //for close(), read(), write()

//set maximal command length to 60 bytes (longest command - start/close <name> - is at most 56 characters)
#define MAX_COMMAND_LENGTH 60

using namespace std;

JoinCommand::JoinCommand() {}


void JoinCommand::execute(vector<string>& args, pthread_t& tid) {
	//get given game name (first argument) and client's sd (second argument)
	string name = args[0];
	int clientB = atoi(args[1].c_str());

	cout << "join command, line " << __LINE__ << "\tclient: " << clientB  << "\ttid: " << tid << "\n"; //TODO

	//join game
	GameInfo* g = GamesInfoLists::getInstance()->joinGame(name, clientB, tid);

	//if returned GameInfo is not the null game (clientA=-2, name=empty string) - ask server to communicate given game
	if (*g != GameInfo("", -2)) {
		handleGame(*g);
	}

	//end thread via return from method - game does not exist or is already being played
}


void JoinCommand::handleGame(GameInfo& g) {
	//Sending 1 to show him he was the first to enter
	int color = 1;
	int n = write(g.getClientA(), &color, sizeof(color));
	if (n == -1) {
		//if problem occured - close sockets and kill thread
		exitThread(g.getClientA(), g.getClientB());
	}

	//Sending 2 to client 2 to show him he was the second to enter
	color = 2;
	n = write(g.getClientB(), &color, sizeof(color));
	if (n == -1) {
		//if problem occured - close sockets and kill thread
		exitThread(g.getClientA(), g.getClientB());
	}

	//play game - accept commands from players
	//loop will keep going until thread is terminated
	while (true) {
		string command = readString(g.getClientA(), g.getClientB());

		//if a problem occurred - close socket and terminate thread (other client has already been notified of disconnection)
		if(command == "") {
			exitThread(g.getClientA(), g.getClientB());
		}

		//split command by space
		vector<string> args = Utility::split(command);

		//make command the first word
		command = args[0];
		//remove command from arguments
		args.erase(args.begin());

		//add the two file descriptors as the last two arguments
		//sd of client 1
		args.insert(args.end(), Utility::toString(g.getClientA()));

		//sd of client 2
		args.insert(args.end(), Utility::toString(g.getClientB())); //add to arguments

		//execute command
		CommandsManager::getInstance()->executeCommand(command, args, g.getTID());

		//switch players and keep communicating game
		g.swapClients();

	}//end of game loop - will end via killing thread ("close" command)
}


string JoinCommand::readString(int client1_sd, int client2_sd) {
	char str[MAX_COMMAND_LENGTH];

	//read string sent
	int n = read(client1_sd, str, MAX_COMMAND_LENGTH);
	if (n == -1) {
		cout << "Error reading string from socket" << endl;
		return "";
	}else if (n == 0) {
		//if no bytes were read from client - client1 has disconnected
		//notify other client (client2) - write (-3) to opponent
		int num = -3;
		int n = write(client2_sd, &num, sizeof(num));
		/* NOTICE: when reading a string with 2 players connected, after sending command (string), next move is read (int)
		THEREFOR we can send (-3) as an error code (just like before).
		In general - client sends both string and integer, but reads integers only */

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing string to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return "";
	}

	//otherwise - all is well, return string read row
	return string(str);
}


void JoinCommand::exitThread(int client1_sd, int client2_sd) {
	//close socket (if a socket is closed - does nothing)
	close(client1_sd);
	close(client2_sd);

	//exit thread
	pthread_exit(NULL);
}
