#include "Command.hpp"
#include "Channel.hpp"

void message_kick(Client *client, Channel *channel, Client * clientsToKick, std::string message)
{
	std::map<int, Client *>clients = channel->getClients();

	for (std::map<int, Client *>::iterator it_channel = clients.begin(); it_channel != clients.end(); ++it_channel)
		// (*it_channel).second->sendTo("KICK " + channel->getName() + " " + clientsToKick->getNickname() + " " + message);
		(*it_channel).second->sendFrom(client, "KICK " + channel->getName() + " " + clientsToKick->getNickname() + " " + message);

}

void execute_kick(Client *client, std::vector<Channel*> channels, std::vector<Client*> clients, Command *command)
{

	std::string message = command->getEndParam();
	if (channels.size() == 1)
	{
		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
			message_kick(client, channels[0], *it, message);
		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			channels[0]->removeClient(*it);
		}
	}
	else
	{
		std::vector<Client*>::iterator client_it = clients.begin();
		for (std::vector<Channel*>::iterator channel_it = channels.begin(); channel_it != channels.end(); ++channel_it)
		{
				message_kick(client, *channel_it, *client_it, message);
				(*channel_it)->removeClient(*client_it);
				++client_it;
		}
	}

}


void parsing_kick(Command *command, Client *client, Server *server, std::vector<std::string> params)
{
	// check if the command has enough params
	if (params.size() < 4)
		return client->sendReply(ERR_NEEDMOREPARAMS(params[0]));

	// check if the channel name is valid -> mask = #
	std::vector<std::string> channels_name = command->multipleParams(params[1]);
	for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); ++it)
	{
		if ((*it)[0] != '#')
			return client->sendReply(ERR_BADCHANMASK(*it));
	}

	// check if the channel exists
	for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); ++it)
	{
		if (!server->getChannel(*it))
			return client->sendReply(ERR_NOSUCHCHANNEL(*it));
	}

	// check if client exist nickname
	//  1. check if the nickname is in the server
	std::vector<std::string> clients_name = command->multipleParams(params[2]);
	for (std::vector<std::string>::iterator it = clients_name.begin(); it != clients_name.end(); ++it)
	{
		if (!server->getClient(*it))
			return client->sendReply(ERR_NOTONCHANNEL(*it));
	}

	std::vector<Channel*> channels;
	for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); ++it)
		channels.push_back(server->getChannel(*it));

	std::vector<Client*> clients;
	for (std::vector<std::string>::iterator it = clients_name.begin(); it != clients_name.end(); ++it)
		clients.push_back(server->getClient(*it));

	// check if the client is in the channel
	// 1. if there is only one channel, check if the clients are in the channel
	// 2. if there is more than one channel, check if clients are with their channel
	if (channels.size() == 1)
	{
		for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			if (!channels[0]->isClientInChannel(*it))
				return client->sendReply(ERR_NOTONCHANNEL((*it)->getNickname()));
		}
	}
	else
	{
		if (channels.size() > clients.size())
			return client->sendReply(ERR_NEEDMOREPARAMS(params[0]));
		else
		{
			// inverse channels and clients
			std::vector<Client*>:: iterator clients_it = clients.begin();
			for (std::vector<Channel*>::iterator channels_it = channels.begin(); channels_it != channels.end(); ++channels_it)
			{
				if (!(*channels_it)->isClientInChannel(*clients_it))
					return client->sendReply(ERR_NOTONCHANNEL((*channels_it)->getName()));
				++clients_it;
			}
		}
	}
		
	execute_kick(client, channels, clients, command);
}

void KICK(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();
	std::vector<std::string> params = command->getParameters();

	parsing_kick(command, client, server, params);
}










/*

18:41:08:   <--{5}[KICK #42 ema :
18:41:08:-->   {5}[:pepe!uxlebaux@127.0.0.1 KICK #42 ema 
18:41:08:-->   {6}[:ema!uxlebaux@127.0.0.1 KICK #42 ema 


[2023-03-28 18:42:02] 5 < KICK #42 ema :
[2023-03-28 18:42:02] 4 > :pepe!uxlebaux@localhost KICK #42 ema :
[2023-03-28 18:42:02] 5 > :pepe!uxlebaux@localhost KICK #42 ema :



*/