#include "Channel.hpp"

Channel::Channel() {}
Channel::~Channel() {
	for (std::map<int, Client*>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
		delete it->second;

	this->_clients.clear();
}
Channel::Channel(const std::string& name) : _name(name), _topic(""), _key("") {}

void	Channel::setName(std::string name) { this->_name = name; }
void	Channel::setKey(std::string key) { this->_key = key; }
void	Channel::setTopic(std::string topic) { this->_topic = topic; }

bool	Channel::isClientInChannel(Client* client)
{ 
	return this->_clients.find(client->getFd()) != this->_clients.end(); 
}

void	Channel::addClient(Client* client) { 
	this->_clients[client->getFd()] = client; 
	client->addChannel(this);
}

bool 	Channel::removeClient(Client* client) {
	client->leaveChannel(this);
	return this->_clients.erase(client->getFd());
}

std::map<int, Client*>&	Channel::getClients() { return this->_clients; }

std::string&	Channel::getName() { return this->_name; }
std::string&	Channel::getTopic() { return this->_topic; }
std::string&	Channel::getKey() { return this->_key; }
