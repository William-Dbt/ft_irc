#include <iostream>
#include <algorithm>
#include <sstream>
#include <sys/socket.h>
#include "utils.hpp"
#include "Client.hpp"

Client::Client(const int& fd, const std::string& host, Server* server) : status(COMMING),
																		 _lastPing(std::time(NULL)),
																		 _fd(fd),
																		 _host(host),
																		 _server(server) {}

Client::~Client() {
	if (this->_fd != -1)
		close(this->_fd);
}

void	Client::connectToClient() {
	reply(1, *this, this->getPrefix());
	reply(2, *this, "TotIrc", "1.0");
	reply(3, *this, getCurrentDateTime());
	reply(4, *this, "TotIrc", "1.0", this->_server->getConfig().get("user_mods"), this->_server->getConfig().get("channel_mods"));
}

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
bool	Client::getBaseInfos(std::string entry) {
	size_t		pos = 0;
	size_t		lastPos;
	std::string	buffer;

	while (pos != entry.size()) {
		lastPos = entry.find("\r\n", pos);
		buffer = entry.substr(pos, lastPos);
		buffer.erase(lastPos - pos);
		pos = lastPos + 2;
		if (buffer.find("CAP LS") != std::string::npos) // Skip the first line (doesn't know what is it for)
			continue ;

		setBaseInfo(buffer, this->_server->getPassword());
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

void	Client::send(std::string message) {
	message.append("\r\n");
	::send(this->_fd, message.c_str(), message.size(), MSG_NOSIGNAL);
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

void	Client::setLastPing(time_t time) {
	this->_lastPing = time;
}

time_t&	Client::getLastPing() {
	return this->_lastPing;
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
