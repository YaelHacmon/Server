#include "CommunicationManager.h"
#include "../include/Server.h"
#include "../include/ClientHandler.h"
#include "../include/CommandsManager.h"

using namespace std;

CommunicationManager *CommunicationManager::instance_ = 0;

CommunicationManager::CommunicationManager() {
	//create CommandManager
	CommandsManager cmd;

	//create ClientHandler, pass commands manager to it
	handler_ = new ClientHandler(cmd);

	//create server, pass configuration file path to it
	server_ = new Server("server_config.txt");
}

//empty destructor - no memory to free at real instance
CommunicationManager::~CommunicationManager() {
	delete handler_;
	delete server_;
}

CommunicationManager *CommunicationManager::getInstance()
{
	if (instance_ == 0)
	{
		instance_ = new CommunicationManager;
	}
	return instance_;
}


void CommunicationManager::resetInstance()
{
	delete instance_; // REM : it works even if the pointer is NULL (does nothing then)
	instance_ = NULL; // so GetInstance will still work.
}

int CommunicationManager::readNum(int client1_sd, int client2_sd) {
	return server_->readNum(client1_sd, client2_sd);
}

int CommunicationManager::writeNum(int num, int client1_sd, int client2_sd) {
	return server_->writeNum(num, client1_sd, client2_sd);
}

int CommunicationManager::writeNum(int num, int client_sd) {
	return server_->writeNum(num, client_sd);
}

std::string CommunicationManager::readString(int client_sd) {
	return server_->readString(client_sd);
}

std::string CommunicationManager::readString(int client1_sd, int client2_sd) {
	return server_->readString(client1_sd, client2_sd);
}

int CommunicationManager::writeString(string &s, int client_sd) {
	return server_->writeString(s, client_sd);
}

Server& CommunicationManager::getServer() {
	return *server_;
}

void CommunicationManager::exitThread(int client1_sd, int client2_sd) {
	server_->exitThread(client1_sd, client2_sd);
}

void CommunicationManager::handleClient(int client_sd) {
	handler_->handleClient(client_sd);
}

void CommunicationManager::handleGame(GameInfo& g) {
	handler_->handleGame(g);
}
