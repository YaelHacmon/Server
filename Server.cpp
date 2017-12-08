/*
 * server.cpp
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
#define MAX_CONNECTED_CLIENTS 10

Server::Server(int port): port(port), serverSocket(0){
	cout<<"Server initialized";
}

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

	cout<<"Server initialized";
}

void Server::start(){
	//Creating the socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket == -1){
		throw "Error opening socket";
	}

	cout<<"opened socket\n";

	//Creating socket address variable for binding
	struct sockaddr_in serverAddress;

	cout<<"created socket address variable for binding\n";

	// Assign a local address to the socket
	bzero((void *)&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	//Gets connections
	serverAddress.sin_addr.s_addr= INADDR_ANY;
	serverAddress.sin_port = htons(port);

	cout<<"\t initialized to 0\n";
	cout<<"serverSocket: " <<serverSocket << "\tserverPort: " << port << endl;

	//binding
	if (bind(serverSocket, (struct sockaddr* )&serverAddress, sizeof(serverAddress)) == -1){
		throw "Error on binding";
	}

	cout<<"binded\n";

	//start listening for clients
	listen(serverSocket, MAX_CONNECTED_CLIENTS);
	cout<< "Waiting for connections\n";

	//declare clients' address
	struct sockaddr_in client1Address, client2Address;
	socklen_t client1AddressLen, client2AddressLen;
	int client1_sd, client2_sd;

	//initialize the addresses - to allow for using 'accept' every time
	client1AddressLen = sizeof((struct sockaddr*) &client1Address);
	client2AddressLen = sizeof((struct sockaddr*) &client2Address);

	cout<<"declared client's address\n";

	//if game ended, start a new one
	while(true){
		//Accepting first client
		client1_sd = accept(serverSocket, (struct sockaddr* )&client1Address, &client1AddressLen);
		cout<< "Client 1 entered!";

		if (client1_sd == -1){
			throw "Error on accept";
		}

		//Accepting second client
		client2_sd = accept(serverSocket, (struct sockaddr* )&client2Address, &client2AddressLen);
		cout<<"Client 2 entered!";

		if (client2_sd == -1){
			throw "Error on accept";
		}

		//Sending 1 to to show him he is the first to enter
		int color = 1;
		int n = write(client1_sd, &color, sizeof(color));
		if (n == -1) {
			cout << "Error writing to socket" << endl;
			return;
		}

		//Sending 2 to him to show him he is the second to enter
		color = 2;
		n = write(client2_sd, &color, sizeof(color));
		if (n == -1) {
			cout << "Error writing to socket" << endl;
			return;
		}

		//handle the clients
		handleClients(client1_sd, client2_sd);

		cout << "finished handeling clients\n";
	} //end big loop
} //end function


void Server::handleClients(int client1_sd, int client2_sd) {
	//declare row, column and temporary variable (avoid redeclaring at each iteration)
	int row, column, temp_sd;

	//while game has not ended, and both clients are connected - play turns
	while(true){
		//read row - first number sent
		row = readNum(client1_sd, client2_sd);
		//if problem occurred - return from function (break loop) to accept new clients
		if (row == -1) {
			break;
		}

		//if game is over (-2) - close and wait for new clients
		if(row == -2){
			cout << "Ending game\n" << endl;
			close(client1_sd);
			close(client2_sd);
			break;
		}

		//write row to other player
		//if an error occurred - break loop (writeNum returns 1 if ok, 0 for an error)
		if(!writeNum(row, client1_sd, client2_sd)) {
			break;
		}

		//if other player made a move (did not send -1) - read and write column of player's move
		if (row != -1) {
			//read column
			column = readNum(client1_sd, client2_sd);
			//if problem occurred - return from function (break loop) to accept new clients
			if (column == -1) {
				break;
			}

			//write column
			//if an error occurred - break loop (writeNum returns 1 if ok, 0 for an error)
			if(!writeNum(column, client1_sd, client2_sd)) {
				break;
			}
		}

		//switch players and keep on playing
		temp_sd = client1_sd;
		client1_sd = client2_sd;
		client2_sd = temp_sd;
	} //end small loop
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

		//close clients
		close(client1_sd);
		close(client2_sd);

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

		//close clients
		close(client1_sd);
		close(client2_sd);

		return 0;
	}

	//all went well - return 1
	return 1;
}
