#ifndef COMMAND_H_
#define COMMAND_H_

#include <vector>
#include <string>

/**
 * Represents a command from the client (sender, creator) to the server (receiver, invoker)
 */
class Command {
public:
	/**
	 * Constructor, since both CommunicationManager and GamesInfoLists are singleton pattern designs,
	 * no parameters are needed.
	 */
	Command() {};

	//virtual destructor
	virtual ~Command() {}

	/**
	 * Executes the action of this command.
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 */
	virtual void execute(std::vector<std::string>& args) = 0;
};

#endif /* COMMAND_H_ */
