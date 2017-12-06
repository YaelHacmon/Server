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
	cout << "constructed\n";

	server.start();

	cout << "started\n";
	return 1;
}

