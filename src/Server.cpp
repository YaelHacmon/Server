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


