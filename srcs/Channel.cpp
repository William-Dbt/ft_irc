#include "Channel.hpp"

Channel::Channel() {}
Channel::~Channel() {}

Channel::Channel(const std::string& name) : _name(name) {}

void	Channel::addClient(Client* client) {
	_clients[client->getFd()] = client;
}


std::string&	Channel::getName() { return _name; }
std::string&	Channel::getTopic() { return _topic; }
std::string&	Channel::getKey() { return _key; }

std::map<int, Client*>&	Channel::getClients() {
	return _clients;
}
