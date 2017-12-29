#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include "Server.h"
#include <vector>
#include <string>

class ClientHandler {
public:
	/**
	 * C'tor of handler.
	 */
	ClientHandler();

	/**
	 * Handles the initial communication with a client: asking to start\join a game and accepting answers
	 * Function must be static to be passed to pthread_create()
	 *
	 * @param client_sd socket descriptor of client
	 */
	static void* handle(void* client_sd);

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
