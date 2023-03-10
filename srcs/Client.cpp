#include <iostream>
#include <algorithm>
#include <sstream>
#include <sys/socket.h>
#include "Client.hpp"
#include "replies.hpp"

Client::Client(const int& fd, const std::string& host) :	status(COMMING),
															_lastPing(std::time(NULL)),
															_fd(fd),
															_host(host) {}

Client::~Client() {
	if (this->_fd != -1)
		close(this->_fd);
}

void Client::connectToClient(Server &server)
{
	this->sendReply(RPL_WELCOME(this->getNickname(), this->getUsername(), this->getHost()));
	this->sendReply(RPL_YOURHOST(server.getConfig().get("server_name"), server.getConfig().get("version")));
	this->sendReply(RPL_CREATED(std::string("today")));
	this->sendReply(RPL_MYINFO(server.getConfig().get("server_name"), server.getConfig().get("version"), "wi", "5"));

/*  (ignore) FOR TESTS ERR
	this->sendReply(ERR_NOSUCHNICK(this->getNickname()));
	this->sendReply(ERR_NOSUCHSERVER(this->getNickname()));
	this->sendReply(ERR_NOSUCHCHANNEL(this->getNickname()));
	this->sendReply(ERR_CANNOTSENDTOCHAN(this->getNickname()));
	this->sendReply(ERR_TOOMANYCHANNELS(this->getNickname()));
	this->sendReply(ERR_TOOMANYTARGETS(this->getNickname()));
	this->sendReply(ERR_NOORIGIN());
	this->sendReply(ERR_NORECIPIENT(this->getNickname()));
	this->sendReply(ERR_NOTOPLEVEL(this->getNickname()));
	this->sendReply(ERR_WILDTOPLEVEL(this->getNickname()));
	this->sendReply(ERR_NOTEXTTOSEND());
	this->sendReply(ERR_NOMOTD());
	this->sendReply(ERR_NONICKNAMEGIVEN());
	this->sendReply(ERR_ERRONEUSNICKNAME(this->getNickname()));
	this->sendReply(ERR_NICKNAMEINUSE(this->getNickname()));
	this->sendReply(ERR_NICKCOLLISION(this->getNickname(), this->getUsername(), this->getHost()));
	this->sendReply(ERR_UNAVAILRESOURCE(this->getNickname()));
	this->sendReply(ERR_USERNOTINCHANNEL(this->getNickname(), "#channel"));
	this->sendReply(ERR_NOTONCHANNEL(std::string("#channel")));
	this->sendReply(ERR_USERONCHANNEL(this->getNickname(), std::string("#channel")));
	this->sendReply(ERR_NEEDMOREPARAMS(this->getNickname()));
	this->sendReply(ERR_ALREADYREGISTRED());
	this->sendReply(ERR_PASSWDMISMATCH());
	this->sendReply(ERR_CHANNELISFULL(std::string("#channel")));
	this->sendReply(ERR_INVITEONLYCHAN(std::string("#channel")));
	this->sendReply(ERR_BANNEDFROMCHAN(std::string("#channel")));
	this->sendReply(ERR_BADCHANNELKEY(std::string("#channel")));
	this->sendReply(ERR_BADCHANMASK(std::string("#channel")));
	this->sendReply(ERR_NOCHANMODES(std::string("#channel")));
	this->sendReply(ERR_NOPRIVILEGES());
	this->sendReply(ERR_CHANOPRIVSNEEDED(std::string("#channel")));
	this->sendReply(ERR_CANTKILLSERVER());
	this->sendReply(ERR_RESTRICTED());
	this->sendReply(ERR_NOOPERHOST());
	this->sendReply(ERR_UMODEUNKNOWNFLAG());
	this->sendReply(ERR_USERSDONTMATCH());
*/
/*	(ignore) FOR TESTS RPL
	this->sendReply(RPL_REHASHING(std::string("server_name")));
	this->sendReply(RPL_VERSION(server.getConfig().get("server_name"), server.getConfig().get("version"), "user", "host"));
	this->sendReply(RPL_MOTDSTART(server.getConfig().get("server_name")));
	this->sendReply(RPL_MOTD(server.getConfig().get("motd")));
	this->sendReply(RPL_ENDOFMOTD());
	this->sendReply(RPL_AWAY(this->getNickname(), this->getUsername()));
	this->sendReply(RPL_INVITING(this->getNickname(), this->getUsername()));
	this->sendReply(RPL_NOTOPIC(std::string("#channel")));
	this->sendReply(RPL_TOPIC(std::string("#channel"), std::string("topic")));
	this->sendReply(RPL_UMODEIS(std::string("+i")));
	this->sendReply(RPL_YOUREOPER());
*/
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
bool	Client::getBaseInfos(Server* server, std::string entry) {
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

void	Client::send(std::string message) {
	message.append("\r\n");
	::send(this->_fd, message.c_str(), message.size(), MSG_NOSIGNAL);

	if (DEBUG)
		std::cout << KBOLD << ">> [" << KRESET << KCYN << message << KRESET << std::endl;
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
