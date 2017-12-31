#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include "CommandsManager.h""
#include <vector>
#include <string>

class ClientHandler {
public:
	/**
	 * C'tor of handler.
	 * @param cmd command manager to manage client by
	 */
	ClientHandler(CommandsManager& cmd);

	/**
	 * Handles the g client of this handler, using the given server.
	 * @param client_sd socket descriptor of client
	 */
	void handleClient(int client_sd);

	/**
	 * Handles the full game logic between two clients.
	 */
	void handleGame(GameInfo& g);

private:
	//manager of commands
	CommandsManager commandManager_;

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
