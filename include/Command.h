#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>
#include <string>
#include "Server.h"

/**
 * Represents a command from the client (sender, creator) to the server (receiver, invoker)
 */
class Command {
public:
	/**
	 * Constructor taking the server to be executed on, by reference.
	 * @param server to apply command to
	 * @param list manager of list of games
	 */
	Command(Server &server, GamesInfoLists& list): server_(server), list_(list) {};

	//virtual destructor
	virtual ~Command() {}

	/**
	 * Executes the action of this command.
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 */
	virtual void execute(std::vector<std::string> args) = 0;

protected:
	Server server_;
	GamesInfoLists list_;
};

#endif /* COMMAND_H_ */
