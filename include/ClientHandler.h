#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include "Server.h"
#include <vector>
#include <string>

class ClientHandler {
public:
	/**
	 * C'tor of handler.
	 * @param s server through which handling is done
	 * @param client_sd socket descriptor of client to handle
	 */
	ClientHandler(Server& s, int client_sd);

	/**
	 * Handles the g client of this handler, using the given server.
	 */
	void handle();

private:
	//server to use in handling client
	Server server_;

	//client's socket descriptor
	int client_;

	/**
	 * Helper function for splitting a command by space
	 */
	vector<string> split(string& line);

	/**
	 * Helper function for cleaner code: converts integer to string
	 */
	string toString(int a);
};


#endif /* CLIENTHANDLER_H_ */
