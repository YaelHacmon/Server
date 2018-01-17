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
#define THREADS_NUM 5
//set maximal command length to 60 bytes (longest command - start/close <name> - is at most 56 characters)
#define MAX_COMMAND_LENGTH 60

ClientHandler* Server::handler_ = 0;

using namespace std;

//vector will be initialized via default c'tor
Server::Server(const string& fileName, ClientHandler& ch): serverSocket(0), pool_(THREADS_NUM) {
	handler_ = &ch;

	//initialize info for acceptClients
	info_.socket = serverSocket;
	info_.tPool = &pool_;

	//pool_.addTask(new Task(handleSingleClient, (void *)3)); //TODO

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

	cout << "Server\t" << __LINE__ << endl; //TODO
	cout << "Server\t" <<  &pool_ << "\t"<< __LINE__ << endl; //TODO
	cout << "Server\t" <<  info_.tPool << "\t"<< __LINE__ << endl; //TODO

	//create thread for accepting clients
	//int rc = pthread_create(&serverThreadId, NULL, acceptClients, (void *)&info_); //does not work TODO
	int rc = pthread_create(&serverThreadId, NULL, acceptClients, (void *)&serverSocket); //does work TODO
	cout << "Server\t" << __LINE__ << endl; //TODO
	if (rc) {
		cout << "Server\t" << __LINE__ << endl; //TODO
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
	pool_.terminate();

	//kill thread of accept()
	pthread_cancel(serverThreadId);

	//close server socket
	close(serverSocket);
	//exit server by ending stop() method
}


void* Server::acceptClients(void* socket) {
	cout << "Server\t" << socket << __LINE__ << endl; //TODO
	Server::AcceptClientInfo* info = (Server::AcceptClientInfo*) socket;

	cout << "Server\t" << info->socket <<"\t"<< __LINE__ << endl; //TODO
	cout << "Server\t" <<  info->tPool << "\t"<< __LINE__ << endl; //TODO

	//long* serverSocket = (long*) socket; TODO
	// Define the client socket's structures
	struct sockaddr_in clientAddress;
	//initialize the addresses - to allow for using 'accept' every time
	socklen_t clientAddressLen = sizeof(clientAddress);

	while (true) {
		// Accept a new client connection
		int clientSocket = accept(info->socket, (struct sockaddr *)&clientAddress, &clientAddressLen);

		if (clientSocket == -1) {
			throw "Error on accept";
		}

		cout << "Server\t" <<  info << "\t"<< __LINE__ << endl; //TODO
		cout << "Server\t" <<  info->tPool << "\t"<< __LINE__ << endl; //TODO

		cout << "Server\t" << "\t"<< __LINE__ << endl; //TODO

		//insert new task into thread pool queue
		info->tPool->addTask(new Task(handleSingleClient, (void *)&clientSocket));

		cout << "Server\t" << __LINE__ << endl; //TODO

		//TODO
//		//short thread   no need to keep thread id
//		pthread_t threadId;
//		int rc = pthread_create(&threadId, NULL, handleSingleClient, (void *)&clientSocket);
//
//		if (rc) {
//			cout << "Error: unable to create thread, " << rc << endl;
//			pthread_exit(NULL);
//		}

		//keep on accepting
	}

	//must return something - non-void
	return NULL;
}


void* Server::handleSingleClient(void* info) {
	//cast to long and then to int (as instructed in class - problem with casting stright to int)
	Server::ClientHandleInfo* clientInfo = (Server::ClientHandleInfo*) info;

	cout << "Server\t" << __LINE__ << endl; //TODO

	//handle client
	getHandler().handleClient(clientInfo->socket, clientInfo->tid);

	cout << "Server\t" << __LINE__ << endl; //TODO

	//must return something - non-void
	return NULL;
}

ClientHandler& Server::getHandler() {
	return *handler_;
}
