#include "Channel.hpp"

Channel::Channel() {}
Channel::~Channel() {}
Channel::Channel(const std::string &name) : _name(name), _topic(""), _key("") {}

void Channel::setName(std::string name) { this->_name = name; }
void Channel::setKey(std::string key) { this->_key = key; }
void Channel::setTopic(std::string topic) { this->_topic = topic; }

bool Channel::isClientInChannel(Client *client) { return this->_clients.find(client->getFd()) != this->_clients.end(); }
void Channel::addClient(Client *client) { this->_clients[client->getFd()] = client; }
bool Channel::removeClient(Client *client) { return this->_clients.erase(client->getFd()); }

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
	std::map<int, Client *>::iterator it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++)
		nicknames += (*it).second->getNickname() + " ";
	return nicknames;
}
std::string &Channel::getName() { return this->_name; }
std::string &Channel::getTopic() { return this->_topic; }
std::string &Channel::getKey() { return this->_key; }
