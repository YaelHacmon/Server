/*
 * main.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: yael
 */


#include <iostream>
#include "Server.h"
using namespace std;

int main(){
	Server server("config.txt");

	try {
		server.start();
	} catch (const char *msg) {
		cout << "Cannot start server. Reason: " << msg << endl;
		exit(-1);
	}

	return 1;
}

