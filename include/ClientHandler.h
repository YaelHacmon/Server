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
	ClientHandler(Server& s);

	/**
	 * Handles the g client of this handler, using the given server.
	 * @param client_sd socket descriptor of client
	 */
	static void handle(int client_sd);

private:
	//server to use in handling client
	Server server_;

	//manager of commands
	CommandsManager commandManager_;

	//handles the lists of existing games and their information
	GamesInfoLists gameList_;

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
