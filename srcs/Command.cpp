#include <iostream>
#include "Command.hpp"

Command::Command(Client* client, std::string line) : _client(client), _commandLine(line) {
	size_t						pos = 0;
	size_t						lastPos;

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
	std::vector<std::string>::iterator it;

	std::cout << "[" << this->_client->getFd() << "] argv: ";
	for (it = this->_commandValues.begin(); it != this->_commandValues.end(); it++)
		std::cout << *it << '|';

	std::cout << std::endl;
}
