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

Server::Server(char* fileName): serverSocket(0){
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
	config.open("config.txt", std::fstream::in);

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
		client1_sd = accept(serverSocket, (struct sockaddr* )&client1Address, &client1AddressLen);
		cout<< "Client 1 entered!";

		//Sending 1 to to show him he is the first to enter
		//TODO - why not use write/read? then just send integers...
		/*buffer[0] = '1';
		send(client1_sd,buffer,1024,0);*/

		int color = 1;
		int n = write(client1_sd, &color, sizeof(color));
		if (n == -1) {
			cout << "Error writing to socket" << endl;
			return;
		}

		//Accepting second client
		client2_sd = accept(serverSocket, (struct sockaddr* )&client2Address, &client2AddressLen);
		cout<<"Client 2 entered!";

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
			n = read(client1_sd, &row, sizeof(row));
			if (n == -1) {
				cout << "Error reading row from socket" << endl;
				return;
			}

			//if game is over (-2) - close and wait for new clients
			if(row == -2){
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

	} //end big loop

} //end function
