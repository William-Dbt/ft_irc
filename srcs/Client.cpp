#include <iostream>
#include <algorithm>
#include <sstream>
#include <fcntl.h>
#include <sys/socket.h>
#include "utils.hpp"
#include "Client.hpp"
#include "replies.hpp"

Client::Client(const int& fd, const std::string& host, Server* server) : status(COMMING),
																		 _lastPing(std::time(NULL)),
																		 _fd(fd),
																		 _host(host),
																		 _server(server) {
	fcntl(fd, F_SETFL, O_NONBLOCK);
	// All of this functions are prototyped in Command.hpp
	this->_commands["PASS"] = PASS;
	this->_commands["NICK"] = NICK;
	this->_commands["USER"] = USER;
	this->_commands["MODE"] = MODE;
	this->_commands["OPER"] = OPER;
	this->_commands["PING"] = PING;
	this->_commands["PONG"] = PONG;
	this->_commands["QUIT"] = QUIT;
	this->_commands["motd"] = MOTD;
	this->_commands["version"] = VERSION;
}

Client::~Client() {
	if (this->_fd != -1)
		close(this->_fd);
}

void Client::connectToClient(Server &server)
{
	this->sendReply(RPL_WELCOME(this->getNickname(), this->getUsername(), this->getHost()));
	this->sendReply(RPL_YOURHOST(server.getConfig().get("server_name"), server.getConfig().get("version")));
	this->sendReply(RPL_CREATED(getCurrentDateTime()));
	this->sendReply(RPL_MYINFO(server.getConfig().get("server_name"), server.getConfig().get("version"), server.getConfig().get("user_mods"), server.getConfig().get("channel_mods")));
}

void	Client::send(std::string message) {
	message.append("\r\n");
	::send(this->_fd, message.c_str(), message.size(), MSG_NOSIGNAL);

	if (DEBUG)
		std::cout << KGRAY << getCurrentDateTime(0,0) << KRESET
			<< KBOLD << "-->   " << KGRAY << "{"<< getFd() << "}" << KRESET 
			<< KBOLD << "[" << KRESET
			<< KCYN << message << KRESET
			<< std::endl;
}

void	Client::sendTo(std::string message) {
	std::string	buffer;

	if (this->status == REGISTER)
		message = ": " + message;
	else {
		buffer = ':' + this->getPrefix() + ' ';
		buffer += message;
		message = buffer;
	}
	send(message);
}

void Client::sendReply(std::string message)
{
	std::string code;
	std::string reply;

	code = message.substr(0, 3);
	reply.append(":" + this->getPrefix() + " " + code + " " + this->getNickname() + " " + message.substr(3, message.size()));
	send(reply);
}

std::string	Client::getPrefix() {
	std::string	buffer;

	if (this->status == REGISTER)
		return "";

	buffer = this->_nickname;
	buffer.append("!");
	buffer += this->_username;
	buffer.append("@");
	buffer += this->_host;
	return buffer;
}

bool	Client::isModeInUse(char mode) {
	std::string::iterator	it;

	if (this->_modes.empty())
		return false;

	if (this->_modes.find(mode) == std::string::npos)
		return false;

	return true;
}

void	Client::addMode(char mode) {
	if (this->_modes.find(mode) != std::string::npos)
		return ;

	this->_modes.push_back(mode);
}

void	Client::removeMode(char mode) {
	if (this->_modes.find(mode) == std::string::npos)
		return ;

	for (std::string::iterator it = this->_modes.begin(); it != this->_modes.end(); it++) {
		if ((*it) == mode) {
			this->_modes.erase(it);
			return ;
		}
	}
}

void	Client::setLastPing(time_t time) {
	this->_lastPing = time;
}

void	Client::setNickname(std::string nickname) {
	this->_nickname = nickname;
}

void	Client::setUsername(std::string username) {
	this->_username = username;
}

void	Client::setRealname(std::string realname) {
	this->_realname = realname;
}

void	Client::setQuitMessage(std::string quitMessage) {
	this->_quitMessage = quitMessage;
}

time_t	Client::getLastPing() {
	return this->_lastPing;
}

int	Client::getFd() {
	return this->_fd;
}

std::string	Client::getHost() {
	return this->_host;
}

std::string	Client::getNickname() {
	if (this->status == COMMING)
		return "*";

	return this->_nickname;
}

std::string	Client::getUsername() {
	return this->_username;
}

std::string	Client::getRealname() {
	return this->_realname;
}

std::string	Client::getUserModes() {
	return this->_modes;
}

std::string	Client::getQuitMessage() {
	if (this->_quitMessage.size())
		return this->_quitMessage;
	
	return "leaving";
}

Server*	Client::getServer() {
	return this->_server;
}

std::map<std::string, void (*)(Command*)>&	Client::getCommands() {
	return this->_commands;
}
