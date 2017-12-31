#include <iostream>
#include <stdlib.h> //for exit(-1)

#include "../include/CommunicationManager.h"
#include "../include/GamesInfoLists.h"
#include "../include/Server.h"

using namespace std;

int main(){
	//get server from communications manager
	CommunicationManager* comm = CommunicationManager::getInstance();
	Server server = comm->getServer();

	try {
		server.start();
	} catch (const char *msg) {
		cout << "\nCannot start server. Reason: " << msg << endl;
		exit(-1);
	}

	//freeing singletons
	CommunicationManager::resetInstance();
	GamesInfoLists::resetInstance();

	return 0;
}

