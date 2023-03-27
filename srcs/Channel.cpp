#include "Channel.hpp"

Channel::Channel() {}
Channel::~Channel() {}
Channel::Channel(const std::string &name) : _name(name), _topic(""), _key("") {}

void Channel::setName(std::string name) { this->_name = name; }
void Channel::setKey(std::string key) { this->_key = key; }
void Channel::setTopic(std::string topic) { this->_topic = topic; }
void Channel::setOperator(Client* client) { this->_operator = client; }
void Channel::setInviteStatus(bool status) { this->_inviteOnly = status; }

bool Channel::isClientInChannel(Client *client) { return this->_clients.find(client->getFd()) != this->_clients.end(); }
bool Channel::isClientOperator(Client* client) {
	if (!this->_operator)
		return false;

	if (this->_operator->getFd() != client->getFd())
		return false;

	return true;
}
void Channel::addClient(Client *client) { this->_clients[client->getFd()] = client; }
bool Channel::removeClient(Client *client) {
	if (isClientOperator(client))
		this->_operator = NULL;

	if (isClientInvited(client))
		this->_invitedList.erase(client->getFd());

	return this->_clients.erase(client->getFd());
}

bool	Channel::isClientInvited(Client* client) {
	if (this->_invitedList.find(client->getFd()) == this->_invitedList.end())
		return false;

	return true;
}

void	Channel::addInvitedClient(Client* client) {
	if (isClientInvited(client))
		return ;

	this->_invitedList[client->getFd()] = client;
}

void	Channel::removeInvitedClient(Client* client) {
	if (!isClientInvited(client))
		return ;

	this->_invitedList.erase(client->getFd());
}

void Channel::sendToAllClients(const std::string &message)
{
	std::map<int, Client *>::iterator it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++)
		(*it).second->send(message);
}

std::map<int, Client *> &Channel::getClients() { return this->_clients; }
std::string Channel::getClientsNicknames()
{
	std::string nicknames = "";
	std::string	nick;
	std::map<int, Client *>::iterator it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (isClientOperator((*it).second))
			nick = "@";

		nick += (*it).second->getNickname();
		nicknames += nick + " ";
		nick.clear();
	}
	return nicknames;
}
std::string &Channel::getName() { return this->_name; }
std::string &Channel::getTopic() { return this->_topic; }
std::string &Channel::getKey() { return this->_key; }
Client*		Channel::getOperator() { return this->_operator; }
bool		Channel::getInviteStatus() { return this->_inviteOnly; }
