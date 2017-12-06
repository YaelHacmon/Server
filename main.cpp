/*
 * main.cpp
 */


#include <iostream>
#include <stdlib.h> //for exit(-1)

#include "Server.h"

using namespace std;

int main(){
	Server server("config.txt");

	try {
		server.start();
	} catch (const char *msg) {
		cout << "\nCannot start server. Reason: " << msg << endl;
		exit(-1);
	}

	return 1;
}

