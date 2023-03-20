#include "Channel.hpp"

Channel::Channel() {}
Channel::~Channel() {}
Channel::Channel(const std::string& name) : _name(name), _topic(""), _key("") {}

void	Channel::setKey(std::string key) { this->_key = key; }
void	Channel::setTopic(std::string topic) { this->_topic = topic; }
void	Channel::setName(std::string name) { this->_name = name; }

void	Channel::addClient(Client* client) { this->_clients[client->getFd()] = client; }
std::map<int, Client*>&	Channel::getClients() { return this->_clients; }

std::string&	Channel::getName() { return this->_name; }
std::string&	Channel::getTopic() { return this->_topic; }
std::string&	Channel::getKey() { return this->_key; }
