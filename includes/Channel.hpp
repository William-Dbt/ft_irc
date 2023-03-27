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
	void	setOperator(Client* client);
	void	setInviteStatus(bool status);

	bool	isClientInChannel(Client* client);
	bool	isClientOperator(Client* client);
	void	addClient(Client* client);
	bool 	removeClient(Client* client);

	bool	isClientInvited(Client* client);
	void	addInvitedClient(Client* client);
	void	removeInvitedClient(Client* client);

	void	sendToAllClients(const std::string& message);

	std::map<int, Client*>&	getClients();
	std::string				getClientsNicknames();
	std::string&			getName();
	std::string&			getTopic();
	std::string&			getKey();
	Client*					getOperator();
	bool					getInviteStatus();

private:
	std::string				_name;
	std::string				_topic;
	std::string				_key;
	Client*					_operator;
	bool					_inviteOnly;
	std::map<int, Client *>	_clients;
	std::map<int, Client*>	_invitedList;
};

# include "Client.hpp"

#endif
