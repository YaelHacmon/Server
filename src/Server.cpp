/*
 * server.cpp
 */

#include "../include/Server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <iterator>
#include <pthread.h>

#define MAX_CONNECTED_CLIENTS 10
//set maximal command length to 60 bytes (longest command - start/close <name> - is at most 56 characters)
#define MAX_COMMAND_LENGTH 60
//set maximal string length (for list_games command) to 1024 bytes (1 kb)
#define MAX_STRING_LENGTH 1024
//TODO - is this okay/enough? -> at least 20 games (longest string sent and recieved)

using namespace std;


//declare CommandManager as class, to avoid loop of headers TODO - right way to do this?
class CommandsManager;

//vector will be initialized via default c'tor
Server::Server(string fileName): serverSocket(0), gameList_(), commandManager_(this, gameList_)  {
	ifstream config;
	config.open(fileName.c_str(), std::fstream::in);

	if (!config) {
		cerr << "Unable to open Server configuration file";
		exit(1);   // call system to stop
	}

	//read server's port from the configuration file
	string portNum;
	config >> portNum;

	//close file
	config.close();

	//assign server port - 	translate port number to integer
	port = atoi(portNum.c_str());
}

void Server::start(){
	//Creating the socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket == -1){
		throw "Error opening socket";
	}

	//Creating socket address variable for binding
	struct sockaddr_in serverAddress;

	// Assign a local address to the socket
	bzero((void *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	//Gets connections
	serverAddress.sin_addr.s_addr= INADDR_ANY;
	serverAddress.sin_port = htons(port);

	//binding
	if (bind(serverSocket, (struct sockaddr* )&serverAddress, sizeof(serverAddress)) == -1){
		throw "Error on binding";
	}

	//start listening for clients
	listen(serverSocket, MAX_CONNECTED_CLIENTS);

	//else - open thread
	//create identifier
	pthread_t tid;

	//TODO - why is this not working?
	int rc = pthread_create(&tid, NULL, acceptClients, NULL);
	if (rc) {
		cout << "Error: unable to create thread, " << rc << endl;
		exit(-1);
	}

	//push identifier into vector
	threads_.push_back(tid);

	//accept input from user
	while(true){
		string input;
		cin >> input;
		//no need for string input validation
		//if input is "exit" - break loop and close all threads
		if (input == "exit") {
			break;
		}
	}

	//loop was broken - input is "exit", server should be closed

	//first, close all sockets (clients will automatically try to read, get 0 and understand that server has disconnected)
	//get vector of all open sockets
	vector<int> sockets = gameList_.getAllOpenSockets();
	//close all sockets
	for (vector<int>::const_iterator iter = sockets.begin(); iter != sockets.end(); iter++) {
		close(*iter);
	}

	//then, kill all threads
	for (vector<pthread_t>::const_iterator iter = threads_.begin(); iter != threads_.end(); iter++) {
		//cancel theard
		pthread_cancel(*iter);
	}

	//exit server by ending start() method

}

void* Server::acceptClients(void* null) {
	//declare clients' address
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLen;
	int client_sd;

	//initialize the addresses - to allow for using 'accept' every time
	clientAddressLen = sizeof((struct sockaddr*) &clientAddress);

	while (true) {
		//Accepting client
		client_sd = accept(serverSocket, (struct sockaddr* )&clientAddress, &clientAddressLen);

		if (client_sd == -1){
			throw "Error on accept";
		}

		//else - open thread
		//create identifier
		pthread_t tid;

		int rc = pthread_create(&tid, NULL, handleSingleClient, (void *)client_sd);
		if (rc) {
			cout << "Error: unable to create thread, " << rc << endl;
			pthread_exit(NULL);
		}

		//push identifier into vector
		threads_.push_back(tid);

		//keep on accepting
	}
}


void* Server::handleSingleClient(void* sd) {
	//cast client sd to integer - first cast to long as instructed
	long temp = (long)sd;
	int client_sd = (int) temp;

	//read string of command from given client
	string command = readString(client_sd);

	//if a problem occurred - close socket and terminate thread
	if(command == NULL) {
		//close socket (if socket is already closed - does nothing)
		close(client_sd);

		//exit thread
		pthread_exit(NULL); //TODO - return?
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


void Server::handleGame(GameInfo& g) {
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
	int temp_sd;
	while (true) {
		string command = readString(g.getClientA(), g.getClientB());

		//if a problem occurred - close socket and terminate thread (other client has already been notified of disconnection)
		if(command == NULL) {
			exitThread(g.getClientA(), g.getClientB());
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


int Server::readNum(int client1_sd, int client2_sd) {
	int num;
	//read number sent
	int n = read(client1_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error reading number from socket" << endl;
		return -1;
	}else if (n == 0) {
		//if no bytes were read from client - client1 has disconnected
		//notify other client (client2) - write (-3) to opponent
		num = -3;
		int n = write(client2_sd, &num, sizeof(num));

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing number to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return -1;
	}

	//otherwise - all is well, return number read row
	return num;
}


int Server::writeNum(int num, int client1_sd, int client2_sd) {
	//write number to opponent
	int n = write(client2_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error writing number to socket" << endl;
		return 0;

	} else if (n == 0) {
		//if no bytes were written to client - client2 has disconnected
		//notify other client (client1) - write (-3) to opponent
		num = -3;
		int n = write(client1_sd, &num, sizeof(num));

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing number to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return 0;
	}

	//all went well - return 1
	return 1;
}


int Server::writeNum(int num, int client_sd) {
	//write number to client
	int n = write(client_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error writing number to socket" << endl;
		return 0;

	} else if (n == 0) {
		//if no bytes were written to client - client has disconnected
		//closing clients will happen when returning with a error

		return 0;
	}

	//all went well - return 1
	return 1;
}


string readString(int client_sd) {
	string str;
	//TODO - is it right to resize each time
	//resize to maximal read string size
	str.resize(MAX_COMMAND_LENGTH);

	//read string sent
	int n = read(client_sd, &str, sizeof(str));
	if (n == -1) {
		cout << "Error reading string from socket" << endl;
		return NULL;
	}else if (n == 0) {
		//if no bytes were read from client - client has disconnected, return NULL (an error occurred)
		//closing clients will happen when returning with a error
		return NULL;
	}

	//otherwise - all is well, return string read row
	return str;
}

string Server::readString(int client1_sd, int client2_sd) {
	string str;
	//TODO - is it right to resize each time
	//resize to maximal read string size
	str.resize(MAX_COMMAND_LENGTH);

	//read string sent
	int n = read(client1_sd, &str, sizeof(str));
	if (n == -1) {
		cout << "Error reading string from socket" << endl;
		return NULL;
	}else if (n == 0) {
		//if no bytes were read from client - client1 has disconnected
		//notify other client (client2) - write (-3) to opponent
		int num = -3;
		int n = write(client2_sd, &num, sizeof(num));
		/* NOTICE: when reading a string with 2 players connected, after sending command (string), next move is read (int)
		THEREFOR we can send (-3) as an error code (just like before).
		In general - client sends both string and integer, but reads integers only */
		//TODO - good design?? :|

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing string to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return NULL;
	}

	//otherwise - all is well, return string read row
	return str;
}

int Server::writeString(string s, int client_sd) {
	//TODO - is it right to resize each time
	//resize to maximal sent string size
	s.resize(MAX_STRING_LENGTH);

	//write number to opponent
	int n = write(client_sd, &s, sizeof(s));
	if (n == -1) {
		cout << "Error writing string to socket" << endl;
		return 0;

	}else if (n == 0) {
		//if no bytes were read from client - client has disconnected, return 0 (an error occurred)
		//closing clients will happen when returning with a error
		return 0;
	}

	//all went well - return 1
	return 1;
}


void Server::exitThread(int client1_sd, int client2_sd) {
	//close socket (if a socket is closed - does nothing)
	close(client1_sd);
	close(client2_sd);

	//exit thread
	pthread_exit(NULL); //TODO - return instead?
}


string Server::toString(int a) {
	//declare stringstream
	stringstream ss;
	//add integer to stream
	ss << a;
	//make stream to string
	string str = ss.str();
	//return string
	return str;
}

vector<string> Server::split(string& line) {
	istringstream iss(line);
	vector<string> args((istream_iterator<string>(iss)), istream_iterator<string>());
	return args;
}
