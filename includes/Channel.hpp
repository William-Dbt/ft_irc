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
	// Channel(const std::string& name, const std::string& key);

	void	addClient(Client* client);

	std::string&			getName();
	std::string&			getTopic();
	std::string&			getKey();
	std::map<int, Client*>&	getClients();


private:
	std::string	_name;
	std::string	_topic;
	std::string	_key;
	std::map<int, Client*>	_clients;
};

# include "Client.hpp"

#endif


