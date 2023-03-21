#include <iostream>
#include "Command.hpp"

Command::Command(Client* client, std::string line) :  _server(client->getServer()), _client(client), _commandLine(line) {
	size_t	pos = 0;
	size_t	lastPos;
	bool	isCarriageReturn = true;

	if (line.find("\r\n") == std::string::npos)
		isCarriageReturn = false;

	while (pos < this->_commandLine.size()) {
		lastPos = this->_commandLine.find(' ', pos);
		if (lastPos == std::string::npos && isCarriageReturn)
			lastPos = this->_commandLine.size() - 2; // -2 Refers to \r\n
		else if (lastPos == std::string::npos)
			lastPos = this->_commandLine.size() - 1;

		this->_commandValues.push_back(this->_commandLine.substr(pos, lastPos - pos));
		if (isCarriageReturn && lastPos != this->_commandLine.size() - 2)
			pos = lastPos + 1;
		else if (!isCarriageReturn && lastPos != this->_commandLine.size() - 1)
			pos = lastPos + 1;
		else
			break ;
	}

	std::vector<std::string>::iterator	it;

	for (it = this->_commandValues.begin(); it != this->_commandValues.end(); it++)
		std::cout << (*it) << std::endl;
}

Command::~Command() {}

void	Command::execute() {
	if (this->_client->status == DISCONNECTED)
		return ;

	if (this->_commandValues[0] != "PASS" && this->_commandValues[0] != "QUIT" && this->_client->status == BADPASSWORD)
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

std::string	Command::getLine() {
	return this->_commandLine;
}

std::string	Command::getEndParam() {
	int		removedChars;
	size_t	pos;

	if (this->_commandLine.find("\r\n") != std::string::npos)
		removedChars = 2;
	else
		removedChars = 1;

	if (this->_commandLine.find(':') == std::string::npos)
		pos = this->_commandValues[0].size() + 1;
	else
		pos = this->_commandLine.rfind(':') + 1;

	return this->_commandLine.substr(pos, this->_commandLine.size() - pos - removedChars);
}

std::vector<std::string>	Command::getValues() {
	return this->_commandValues;
}
