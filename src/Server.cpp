#include "../include/Server.h"
#include "../include/GamesInfoLists.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h> //for multithreading :)


#define MAX_CONNECTED_CLIENTS 10
//set maximal command length to 60 bytes (longest command - start/close <name> - is at most 56 characters)
#define MAX_COMMAND_LENGTH 60

ClientHandler* Server::handler_ = 0;

using namespace std;

//vector will be initialized via default c'tor
Server::Server(const string& fileName, ClientHandler& ch): serverSocket(0) {
	handler_ = &ch;

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

	//acceptClients((void *)&serverSocket); //TODO

	//create thread for accepting clients
	int rc = pthread_create(&serverThreadId, NULL, acceptClients, (void *)&serverSocket);
	if (rc) {
		cout << "Error: unable to create thread, " << rc << endl;
		exit(-1);
	}

	//exit thread - return to main
}


void Server::stop() {
	//first, close all sockets (clients will automatically try to read, get 0 and understand that server has disconnected)
	//get vector of all open sockets
	vector<int> sockets = GamesInfoLists::getInstance()->getAllOpenSockets();
	//close all sockets
	for (vector<int>::const_iterator iter = sockets.begin(); iter != sockets.end(); iter++) {
		close(*iter);
	}

	//then, kill all threads
	vector<pthread_t> threads = GamesInfoLists::getInstance()->getAllThreadIDs();
	for (vector<pthread_t>::const_iterator iter = threads.begin(); iter != threads.end(); iter++) {
		//cancel theard
		pthread_cancel(*iter);
	}

	//kill thread of accept()
	pthread_cancel(serverThreadId);

	//close server socket
	close(serverSocket);
	cout << "Server stopped" << endl;

	//exit server by ending stop() method
}


void* Server::acceptClients(void* socket) {
	cout << "\tin accept clients\n"; //TODO

	long* serverSocket = (long*) socket;
	// Define the client socket's structures
	struct sockaddr_in clientAddress;
	//initialize the addresses - to allow for using 'accept' every time
	socklen_t clientAddressLen = sizeof(clientAddress);

	while (true) {
		cout << "Waiting for client connections..." << endl;

		// Accept a new client connection
		int clientSocket = accept(*serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLen);

		cout << "Client connected" << endl;

		if (clientSocket == -1) {
			cout << "Error on accept\n";
			throw "Error on accept";
		}

		//handleSingleClient((void *)&clientSocket); //TODO

		//short thread   no need to keep thread id
		pthread_t threadId;
		int rc = pthread_create(&threadId, NULL, handleSingleClient, (void *)&clientSocket);

		if (rc) {
			cout << "Error: unable to create thread, " << rc << endl;
			pthread_exit(NULL);
		}

		//keep on accepting
	}

	//must return something - non-void
	return NULL;
}


void* Server::handleSingleClient(void* info) {
	cout << "\tin handle single client\n"; //TODO

	//cast to long and then to int (as instructed in class - problem with casting stright to int)
	Server::ClientHandleInfo* clientInfo = (Server::ClientHandleInfo*) info;

	//handle client
	getHandler().handleClient(clientInfo->socket, clientInfo->tid);

	//must return something - non-void
	return NULL;
}

ClientHandler& Server::getHandler() {
	return *handler_;
}
