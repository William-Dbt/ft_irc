#include <iostream>
#include <algorithm>
#include <sstream>
#include <sys/socket.h>
#include "Client.hpp"

Client::Client(const int& fd, const std::string& host) :	status(COMMING),
															_fd(fd),
															_host(host) {}

Client::~Client() {
	if (this->_fd != -1)
		close(this->_fd);
}

void	Client::connectToClient() {
	/* int									i;
	std::string							buffer;
	std::vector<std::string>::iterator	it;

	for (it = this->_commands.begin(), i = 1; it != this->_commands.end(); it++, i++) {
		std::ostringstream	ss;

		ss << i;

		std::string	code = ss.str();

		// buffer = ':' + this->getPrefix() + ' ';
		// buffer += "00" + code + " ";
		// buffer += (*it) + '\n';
		buffer = ": NICK :" + this->_nickname + '\n';
		send(this->_fd, buffer.c_str(), buffer.size(), 0);
		std::cout << buffer << std::endl;
	} */

	// TODO: send replies to client, RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO
	std::string	buffer;

	buffer = ": NICK :" + this->_nickname + '\n';
	send(this->_fd, buffer.c_str(), buffer.size(), 0);
}

// TODO: Send commands to client within filling values in class
void	Client::setBaseInfo(std::string& entryInfo, std::string& serverPassword) {
	size_t	pos;

	if (this->_password.empty()) {
		this->_password = entryInfo.substr(entryInfo.find(' ') + 1, entryInfo.size());
		if (this->_password.compare(serverPassword) != 0)
			this->_password.clear();
	}
	else if (this->_nickname.empty())
		this->_nickname = entryInfo.substr(entryInfo.find(' ') + 1, entryInfo.size());
	else if (this->_username.empty()) {
		pos = entryInfo.find(' ');
		this->_username = entryInfo.substr(pos + 1, entryInfo.find(' ', pos + 1) - 5); // (-5 refers to the string "USER " before the command)
		this->_realname = entryInfo.substr(entryInfo.find(':') + 1, entryInfo.size());
	}
}

// In this function we'll get the third params that we receive from our irc client
// and save them in our class
// Theses params are : PASS, NICK, USER
// It refers to the first infos of the client that we receive
bool	Client::getBaseInfos(Server* server, std::string entry) {
	size_t		pos = 0;
	size_t		lastPos = 0;
	std::string	buffer;

	entry.erase(std::remove(entry.begin(), entry.end(), '\r'), entry.end()); // Remove all '\r' because we don't want them
	while (pos != entry.size()) {
		lastPos = entry.find('\n', pos);
		buffer = entry.substr(pos, lastPos);
		buffer.erase(lastPos - pos);
		pos = lastPos + 1;
		if (buffer.find("CAP LS") != std::string::npos) // Skip the first line (doesn't know what is it for)
			continue ;

		setBaseInfo(buffer, server->getPassword());
	}
	if (!this->_password.size())
		return false;
	else if (!this->_nickname.size())
		return false;
	else if (!this->_username.size())
		return false;
	else if (!this->_realname.size())
		return false;

	this->status = REGISTER;
	return true;
}

std::string	Client::getPrefix() {
	std::string	buffer;

	buffer = this->_nickname;
	buffer.append("!");
	buffer += this->_username;
	buffer.append("@");
	buffer += this->_host;
	return buffer;
}

int&	Client::getFd() {
	return this->_fd;
}

std::string&	Client::getHost() {
	return this->_host;
}

std::string&	Client::getPassword() {
	return this->_password;
}

std::string&	Client::getNickname() {
	return this->_nickname;
}

std::string&	Client::getUsername() {
	return this->_username;
}

std::string&	Client::getRealname() {
	return this->_realname;
}
