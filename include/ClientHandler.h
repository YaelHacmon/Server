#ifndef CLIENTHANDLER_H_
#define CLIENTHANDLER_H_

#include "CommandsManager.h"

class ClientHandler {
public:
	/**
	 * C'tor of handler.
	 * @param cmd command manager to manage client by
	 */
	ClientHandler();

	/**
	 * Handles the g client of this handler, using the given server.
	 * @param client_sd socket descriptor of client
	 */
	void handleClient(int client_sd);

	/**
	 * Reads string from client.
	 * @return string read if succeeded, an empty string if not
	 */
	string readString(int client_sd);
};


#endif /* CLIENTHANDLER_H_ */
