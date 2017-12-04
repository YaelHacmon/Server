/*
 * server.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: yael
 */


#include "Server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>


using namespace std;
#define MAX_CONNECTED_CLIENTS 2

Server::Server(int port): port(port), serverSocket(0){
	cout<<"Server initialized";
}

Server::Server(char* fileName): serverSocket(0){
	string buffer,dummyLine;
	ifstream config;
	config.open(fileName, std::fstream::in);
	getline(config, dummyLine);

	//dummyLine contains the first line
	getline(config, buffer);

	//erase the "Port = " to get the port itself
	buffer.erase(buffer.begin(), buffer.begin() + strlen("Port = "));

	port = atoi(buffer.c_str());
	config.close();
}
void Server::start(){
	int client1_sd, client2_sd;
	char buffer[1024];

	//clients' address
	struct sockaddr_in client1Address, client2Address;
	socklen_t client1AddressLen, client2AddressLen;

	//cleaning buffer
	memset(&buffer[0], 0, sizeof(buffer));

	//Creating the socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket == -1){
		throw "Error opening socket";
	}

	//Creating socket address variable for binding
	struct sockaddr_in serverAddress;

	//initializing it to 0's
	bzero((void *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	//Gets connections
	serverAddress.sin_addr.s_addr= INADDR_ANY;
	serverAddress.sin_port = htons(port);

	//binding
	if (bind(serverSocket, (struct sockaddr* )&serverAddress,
		sizeof(serverAddress)) == -1){
		throw "Error on binding";
	}

	//if game ended, start a new one
	while(true){
		//start listening for clients
		listen(serverSocket, MAX_CONNECTED_CLIENTS);
		cout<< "Waiting for connections";

		//Accepting first client
		client1_sd = accept(serverSocket,
					(struct sockaddr* )&client1Address,
				 	 &client1AddressLen);
		cout<< "Client 1 entered!";

		//Sending 1 to to show him he is the first to enter
		buffer[0] = '1';
		send(client1_sd,buffer,1024,0);

		//Accepting second client
		client2_sd = accept(serverSocket,
					(struct sockaddr* )&client1Address,
					&client1AddressLen);
		cout<<"Client 2 entered!";

		//Sending 2 to him to show him he is the second to enter
		buffer[0] = '2';
		send(client2_sd,buffer,1024,0);

		while(true){
			memset(&buffer[0], 0, sizeof(buffer));
			//input - client 1
			recv(client1_sd, buffer, 1024, 0);
			if(strcmp(buffer, "End") == 0){
				close(client1_sd);
				close(client2_sd);
				break;
			}

			//return message
			send(client2_sd, buffer, 1024, 0);
			memset(&buffer[0], 0, sizeof(buffer));

			//input - client 2
			recv(client2_sd, buffer, 1024, 0);
			if(strcmp(buffer, "End") == 0){
				close(client1_sd);
				close(client2_sd);
				break;
			}

			//return message
			send(client1_sd, buffer, 1024, 0);
		}
	}

