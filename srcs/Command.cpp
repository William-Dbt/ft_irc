#include <iostream>
#include "Command.hpp"

/*
* commmand constructor
* 1. split the line into parameters
* 2. store the parameters in a vector
*/
Command::Command(Client* client, std::string line) :  _server(client->getServer()), _client(client), _line(line) {
	size_t	pos = 0;
	size_t	lastPos;

	while (pos < this->_line.size()) {
		lastPos = this->_line.find(' ', pos);

		if (lastPos == std::string::npos)
			lastPos = this->_line.size() - 1;

		this->_parameters.push_back(this->_line.substr(pos, lastPos - pos));
		if (lastPos != this->_line.size() - 1)
			pos = lastPos + 1;
		else
			break ;
	}
}

Command::~Command() {}

/*
* command destructor
* 1. check if the client is disconnected
* 2. check if the client is not connected and the command is not PASS or QUIT
* 3. try to execute the command with the client's commands map with function pointers
*/
void	Command::execute() {
	if (this->_client->status == DISCONNECTED)
		return ;

	if (this->_parameters[0] != "PASS" && this->_parameters[0] != "QUIT" && this->_client->status == BADPASSWORD)
		return ;

	if (this->_client->commandBuffer.size())
		this->_client->commandBuffer.clear();

	try {
		this->_client->getCommands().at(this->_parameters[0])(this);
	}
	catch (std::exception & e) {
		this->_client->send("- The command " + this->_parameters[0] + " isn't known by the server " + this->_server->getConfig().get("server_name") + ".");
	}
}

Server*	Command::getServer() {
	return this->_server;
}

Client*	Command::getClient() {
	return this->_client;
}

std::string	Command::getLine() {
	return this->_line;
}

std::string	Command::getEndParam() {
	size_t	pos;

	if (this->_line.find(':') == std::string::npos)
		pos = this->_parameters[0].size() + 1;
	else
		pos = this->_line.rfind(':') + 1;

	return this->_line.substr(pos, this->_line.size() - pos - 1);
}

std::vector<std::string>	Command::getParameters() {
	return this->_parameters;
}
