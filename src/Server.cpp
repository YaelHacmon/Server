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
#define MAX_CONNECTED_CLIENTS 10

using namespace std;

//for cleaner code
typedef Server::Game Game;

//declare CommandManager as class, to avoid loop of headers TODO - right way to do this?
class CommandsManager;


Server::Server(int port): port(port), serverSocket(0){}

Server::Server(string fileName): serverSocket(0){
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

	//declare clients' address
	struct sockaddr_in client1Address, client2Address;
	socklen_t client1AddressLen, client2AddressLen;
	int client1_sd, client2_sd;

	//initialize the addresses - to allow for using 'accept' every time
	client1AddressLen = sizeof((struct sockaddr*) &client1Address);
	client2AddressLen = sizeof((struct sockaddr*) &client2Address);

	//TODO - somehow this also needs to be multithreaded... the two accepts should be separated

	//if game ended, start a new one
	while(true){
		//Accepting first client
		client1_sd = accept(serverSocket, (struct sockaddr* )&client1Address, &client1AddressLen);

		if (client1_sd == -1){
			throw "Error on accept";
		}

		//TODO - I think this should be removed... because at each time we only accept a single client and handle with handleSingleClient()
		//Accepting second client
		client2_sd = accept(serverSocket, (struct sockaddr* )&client2Address, &client2AddressLen);

		if (client2_sd == -1){
			throw "Error on accept";
		}

		//handle the clients - TODO - change to handleSingleClient(), using multithreading
		//handleClients(client1_sd, client2_sd);

	} //end big loop
} //end function


//TODO - this is the old code (what was inside the big while loop - should now be inside playMove
int Server::playMove(int client1_sd, int client2_sd) {
	//read row - first number sent
	int row = readNum(client1_sd, client2_sd);
	//if problem occurred - return from function
	if (row == -1) {
		return -1;
	}

	//if game is over (-2) - return from function TODO - required??
	if(row == -2){
		return -1;
	}

	//write row to other player
	//if an error occurred - return from function
	if(!writeNum(row, client1_sd, client2_sd)) {
		return -1;
	}

	//if other player made a move (did not send -1) - read and write column of player's move
	if (row != -1) {
		//read column
		int column = readNum(client1_sd, client2_sd);
		//if problem occurred - return from function
		if (column == -1) {
			return -1;
		}

		//write column
		//if an error occurred - return from function
		if(!writeNum(column, client1_sd, client2_sd)) {
			return -1;
		}
	}

	//method ended successfully - return 0 (game is not over)
	return 0;
}


string& Server::findGame(int client_sd) {
	//search played games
	for (vector<Game>::const_iterator iter = playingGames_.begin(); iter != playingGames_.end(); iter++) {
		//if given client is one of the playing clients
		if (iter->client1_sd == client_sd || iter->client2_sd == client_sd) {
			//return game name
			return iter->name;
		}
	}

	//search waiting games
	for (vector<Game>::const_iterator iter = waitingGames_.begin(); iter != waitingGames_.end(); iter++) {
		//if given client is the waiting client
		if (iter->client1_sd == client_sd) {
			//return game name
			return iter->name;
		}
	}

	//else - return the empty string
	return "";
}


void Server::handleSingleClient(int client_sd) {
	string command;
	//TODO accept command from client (if he wants to start a game or see the list of games->accept command with game to join)

	//split command by space
	istringstream iss(command);
	vector<string> args((istream_iterator<string>(iss)), istream_iterator<string>());

	//make command the first word
	command = args[0];
	//remove command from arguments
	args.erase(args.begin());

	//add the client's file descriptor as the last arguments
	args.insert(args.end(), toString(client_sd)); //add to arguments

	//execute command
	commandManager_.executeCommand(command, args);

	//TODO - explanation - will either open a game in the game lists, and exit thread, or create new thread to play a game
	//so we don't need to do anything else
	//TODO - do we need pthread_exit()?
}


void Server::handleGame(Game& g) {
	//Sending 1 to show him he was the first to enter
	int color = 1;
	int n = write(g.client1_sd, &color, sizeof(color));
	if (n == -1) {
		cout << "Error writing to socket" << endl;
		return;
	}

	//Sending 2 to client 2 to show him he was the second to enter
	color = 2;
	n = write(g.client2_sd, &color, sizeof(color));
	if (n == -1) {
		cout << "Error writing to socket" << endl;
		return;
	}

	//play game - accept commands from players
	//loop will never be broken, until "close" command will be invoked and close thread     TODO - true?
	int temp_sd;
	while (true) {
		string command;
		//TODO send\receive command via server - receive from client1 - switching is done at end of loop

		//split command by space
		istringstream iss(command);
		vector<string> args((istream_iterator<string>(iss)), istream_iterator<string>());

		//make command the first word
		command = args[0];
		//remove command from arguments
		args.erase(args.begin());

		//add the two file descriptors as the last two arguments
		//sd of client 1
		args.insert(args.end(), toString(g.client1_sd));

		//sd of client 2
		args.insert(args.end(), toString(g.client2_sd)); //add to arguments

		//execute command
		commandManager_.executeCommand(command, args);

		//switch players and keep playing
		temp_sd = g.client1_sd;
		g.client1_sd = g.client2_sd;
		g.client2_sd = temp_sd;
	}
}


int Server::readNum(int client1_sd, int client2_sd) {
	int num;
	//read number sent
	int n = read(client1_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error reading row from socket" << endl;
		return -1;
	}else if (n == 0) {
		//if no bytes were read from client - client1 has disconnected
		//notify other client (client2) - write (-3) to opponent
		num = -3;
		int n = write(client2_sd, &num, sizeof(num));

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing row to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return -1;
	}

	//otherwise - all is well, return read row
	return num;
}


int Server::writeNum(int num, int client1_sd, int client2_sd) {
	//write number to opponent
	int n = write(client2_sd, &num, sizeof(num));
	if (n == -1) {
		cout << "Error writing row to socket" << endl;
		return 0;

	} else if (n == 0) {
		//if no bytes were written to client - client2 has disconnected
		//notify other client (client1) - write (-3) to opponent
		num = -3;
		int n = write(client1_sd, &num, sizeof(num));

		//check that writing succeeded (do not use writeNum to avoid infinite loop)
		if (n == -1) {
			cout << "Error writing row to socket" << endl;
		}

		//closing clients will happen when returning with a error

		return 0;
	}

	//all went well - return 1
	return 1;
}


std::string Server::toString(int a) {
	//declare (empty c'tor)
	stringstream ss;
	//add integer to stream
	ss << a;
	//get contents
	string str = ss.str();

	//return string
	return str;
}


vector<string> Server::split(string& line) {
	istringstream iss(line);
	vector<string> args((istream_iterator<string>(iss)), istream_iterator<string>());
	return args;
}
