#ifndef __CHANNEL_HPP__
# define __CHANNEL_HPP__

# include <iostream>
# include <string>
# include <map>
# include <vector>

class Client;

class Channel
{
public:
	Channel();
	~Channel();
	Channel(const std::string& name);

	void	setName(std::string name);
	void	setKey(std::string key);
	void	setTopic(std::string topic);

	bool	isClientInChannel(Client* client);

	void	addClient(Client* client);
	bool 	removeClient(Client* client);

	std::map<int, Client*>&	getClients();
	std::string&			getName();
	std::string&			getTopic();
	std::string&			getKey();


private:
	std::string	_name;
	std::string	_topic;
	std::string	_key;
	std::map<int, Client*>	_clients;
};

# include "Client.hpp"

#endif
