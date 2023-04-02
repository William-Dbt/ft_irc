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
	// All of this functions are prototyped in Command.hpp
	this->_commands["PASS"] = PASS;
	this->_commands["NICK"] = NICK;
	this->_commands["USER"] = USER;
	this->_commands["OPER"] = OPER;
	this->_commands["MODE"] = MODE;
	this->_commands["QUIT"] = QUIT;
	this->_commands["JOIN"] = JOIN;
	this->_commands["PART"] = PART;
	this->_commands["TOPIC"] = TOPIC;
	this->_commands["INVITE"] = INVITE;
	this->_commands["KICK"] = KICK;
	this->_commands["NAMES"] = NAMES;
	this->_commands["LIST"] = LIST;
	this->_commands["PRIVMSG"] = PRIVMSG;
	this->_commands["NOTICE"] = NOTICE;
	this->_commands["AWAY"] = AWAY;
	this->_commands["motd"] = MOTD;
	this->_commands["version"] = VERSION;
	this->_commands["kill"] = KILL;
	this->_commands["PING"] = PING;
	this->_commands["PONG"] = PONG;
	this->_commands["rehash"] = REHASH;
	this->_commands["die"] = DIE;
	this->_commands["wallops"] = WALLOPS;
}

Client::~Client() {
	if (this->_fd != -1)
		close(this->_fd);
}

void Client::connectToClient(Server &server)
{
	this->setLastPing(time(NULL));
	this->sendReply(RPL_WELCOME(this->getNickname(), this->getUsername(), this->getHost()));
	this->sendReply(RPL_YOURHOST(server.getConfig().get("server_name"), server.getConfig().get("version")));
	this->sendReply(RPL_CREATED(getCurrentDateTime()));
	this->sendReply(RPL_MYINFO(server.getConfig().get("server_name"), server.getConfig().get("version"), server.getConfig().get("user_mods"), "i"));

	Command command(this, "motd");
	MOTD(&command);
}

void	Client::send(std::string message) {
	message.append("\r\n");
	::send(this->_fd, message.c_str(), message.size(), MSG_NOSIGNAL);
	printLog(message, SENDING, this->_fd);
}

/*
* This function is used to send a message to a client.
* called by Server.deleteClient() with "QUIT :Server closed"
*/
void	Client::sendTo(std::string message) {
	std::string	buffer;

	if (this->status < FULLYREGISTER)
		message = ": " + message;
	else {
		buffer = ':' + this->getPrefix() + ' ';
		buffer += message;
		message = buffer;
	}
	send(message);
}

void	Client::sendFrom(Client * senderClient, std::string message) {
	std::string	buffer;

	if (senderClient->status == REGISTER)
		message = ": " + message;
	else {
		buffer = ':' + senderClient->getPrefix() + ' ';
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

	if (this->status < FULLYREGISTER)
		return "";

	buffer = getNickname();
	buffer.append("!");
	buffer += getUsername();
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

void	Client::setAwayMessage(std::string awayMessage) {
	this->_awayMessage = awayMessage;
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
	if (this->_nickname.empty())
		return "*";

	return this->_nickname;
}

std::string	Client::getUsername() {
	if (this->_username.empty())
		return "#";

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

std::string	Client::getAwayMessage() {
	return this->_awayMessage;
}

Server*	Client::getServer() {
	return this->_server;
}


/*
* getCommands
* called by Command.execute()
*/
std::map<std::string, void (*)(Command*)>&	Client::getCommands() {
	return this->_commands;
}
