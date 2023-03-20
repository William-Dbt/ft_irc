#include <iostream>
#include "Command.hpp"

Command::Command(Client* client, std::string line) :  _server(client->getServer()), _client(client), _commandLine(line) {
	size_t	pos = 0;
	size_t	lastPos;

	while (pos < this->_commandLine.size()) {
		lastPos = this->_commandLine.find(' ', pos);
		if (lastPos == std::string::npos)
			lastPos = this->_commandLine.size() - 2; // -2 Refers to \r\n

		this->_commandValues.push_back(this->_commandLine.substr(pos, lastPos - pos));
		if (lastPos != this->_commandLine.size() - 2)
			pos = lastPos + 1;
		else
			break ;
	}
}

Command::~Command() {}

void	Command::execute() {
	if (this->_client->status == DISCONNECTED || this->_client->status == BADPASSWORD)
		return ;

	try {
		this->_client->getCommands().at(this->_commandValues[0])(this);
	}
	catch (std::exception & e) {
		this->_client->send("- The command " + this->_commandValues[0] + " isn't known by the server " + this->_server->getConfig().get("server_name") + ".");
	}
}

Server*	Command::getServer() {
	return this->_server;
}

Client*	Command::getClient() {
	return this->_client;
}

std::string&	Command::getLine() {
	return this->_commandLine;
}

std::vector<std::string>&	Command::getValues() {
	return this->_commandValues;
}
