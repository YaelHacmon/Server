#include <iostream>
#include <stdlib.h> //for exit(-1)

#include "../include/GamesInfoLists.h"
#include "../include/Server.h"

using namespace std;

int main(){
	//create client handler
	ClientHandler handler;

	//create server
	Server server("server_config.txt", handler);

	try {
		//start server - in thread
		server.start();

		//accept input from user
		cout << "Enter exit to stop server" << endl;
		string input;
		while(true){
			cin >> input;
			//no need for string input validation
			//if input is "exit" - break loop and close all threads
			if (input == "exit") {
				break;
			}
			//otherwise - reprint message
			cout << "Enter exit to stop server" << endl;
		}

		//kill server
		server.stop();

	} catch (const char *msg) {
		cout << "Cannot start server. Reason: " << msg << endl;
		exit(-1);
	}

	//free singletons
	GamesInfoLists::resetInstance();
	CommandsManager::resetInstance();

	return 0;
}

