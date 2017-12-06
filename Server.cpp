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
#define MAX_CONNECTED_CLIENTS 2

Server::Server(int port): port(port), serverSocket(0){
	cout<<"Server initialized";
}

Server::Server(string fileName): serverSocket(0){
	//TODO - recommendation - use my code

	/*string buffer,dummyLine;
	ifstream config;
	config.open(fileName, std::fstream::in);
	getline(config, dummyLine);

	//dummyLine contains the first line
	getline(config, buffer);

	//erase the "Port = " to get the port itself
	buffer.erase(buffer.begin(), buffer.begin() + strlen("Port = "));

	port = atoi(buffer.c_str());
	config.close();*/

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
	char buffer[1024];

	//cleaning buffer
	memset(&buffer[0], 0, sizeof(buffer));

	cout<<"cleaned buffer\n";

	//Creating the socket
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket == -1){
		throw "Error opening socket";
	}

	cout<<"opened socket\n";

	//Creating socket address variable for binding
	struct sockaddr_in serverAddress;

	cout<<"created socket address variable for binding\n";

	//initializing it to 0's
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

	//clients' address
	struct sockaddr_in client1Address, client2Address;
	socklen_t client1AddressLen, client2AddressLen;
	int client1_sd, client2_sd;

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
		//TODO - why not use write/read? then just send integers...
		/*buffer[0] = '2';
		send(client2_sd,buffer,1024,0);*/
		color = 2;
		n = write(client2_sd, &color, sizeof(color));
		if (n == -1) {
			cout << "Error writing to socket" << endl;
			return;
		}

		//handle the clients
		handleClients(client1_sd, client2_sd);

	} //end big loop

} //end function


void Server::handleClients(int client1_sd, int client2_sd) {
	//declare row, column and temporary variable (avoid redeclaring at each iteration)
	int row, column, temp_sd;

	while(true){
		/*
		memset(&buffer[0], 0, sizeof(buffer));
		//input - client 1
		recv(client1_sd, buffer, 1024, 0);
		if(strcmp(buffer, "End") == 0){ //TODO - should be using: n = read(..), then if (n==-2) -> exit
			close(client1_sd);
			close(client2_sd);
			break;
		}

		//return message
		//TODO - why not use write/read? then just send integers...
		send(client2_sd, buffer, 1024, 0);
		memset(&buffer[0], 0, sizeof(buffer));


		//input - client 2
		//TODO - why not use write/read? then just send integers...
		recv(client2_sd, buffer, 1024, 0);
		if(strcmp(buffer, "End") == 0){ //TODO - should be using: n = read(..), then if (n==-2) -> exit
			close(client1_sd);
			close(client2_sd);
			break;
		}

		//return message
		//TODO - why not use write/read? then just send integers...
		send(client1_sd, buffer, 1024, 0);*/

		//read row - first number sent
		int n = read(client1_sd, &row, sizeof(row));
		if (n == -1) {
			cout << "Error reading row from socket" << endl;
			return;
		}

		//if game is over (-2) - close and wait for new clients
		if(row == -2){
			cout << "Ending game\n" << endl;
			close(client1_sd);
			close(client2_sd);
			break;
		}

		//if other player had no moves (-1) - send that
		if (row == -1) {
			n = write(client2_sd, &row, sizeof(row));
			if (n == -1) {
				cout << "Error writing 'no moves' to socket" << endl;
				return;
			}
		}
		//else - other player made a move - read column of move and send it to opponent
		else {
			//read column
			n = read(client1_sd, &column, sizeof(column));
			if (n == -1) {
				cout << "Error reading column from socket" << endl;
				return;
			}

			//write row to opponent
			n = write(client2_sd, &row, sizeof(row));
			if (n == -1) {
				cout << "Error writing row to socket" << endl;
				return;
			}

			//write column to opponent
			n = write(client2_sd, &column, sizeof(column));
			if (n == -1) {
				cout << "Error writing column' to socket" << endl;
				return;
			}
		}

		//switch players and keep on playing
		//TODO - recognize that trick from someone?? ;)
		temp_sd = client1_sd;
		client1_sd = client2_sd;
		client2_sd = temp_sd;

	} //end small loop
}
