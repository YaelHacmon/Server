#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "Command.h"
#include <map>

using namespace std;

class CommandsManager {
public:

	static CommandsManager* getInstance();

	// static destructor for avoiding infinite loop when releasing real instance of class
	static void resetInstance();

	/**
	 * Destructor to release map
	 */
	~CommandsManager();

	/**
	 * Executes a command by the given key and arguments
	 * @param command key for map
	 * @param args arguments for command's execution, by the set protocol of the certain instance
	 * @param tid id of current thread
	 */
	void executeCommand(string& command, vector<string>& args, pthread_t& tid);

private:
	static CommandsManager* instance_;
	static pthread_mutex_t lock_;

	// A singleton
	CommandsManager();

	CommandsManager(const CommandsManager &);

	//map of commands, where the key is a string (and value is the Command, of course)
	std::map<std::string, Command*> commandsMap;

};


#endif /* COMMANDMANAGER_H_ */
