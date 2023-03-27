#include "Command.hpp"
#include "Channel.hpp"

void message_kick(Channel *channel, Client * clientsToKick, std::string message, Client *sender)
{
    std::map<int, Client *>clients = channel->getClients();

    for (std::map<int, Client *>::iterator it_channel = clients.begin(); it_channel != clients.end(); ++it_channel)
            (*it_channel).second->sendFrom(sender, "KICK " + channel->getName() + " " + clientsToKick->getNickname() + " " + message);
}

void execute_kick(std::vector<Channel*> channels, std::vector<Client*> clients, Command *command, Client *sender)
{

    std::string message = command->getEndParam();
    for (std::vector<Client*>::iterator client_it = clients.begin(); client_it != clients.end(); ++client_it)
    {
        for (std::vector<Channel*>::iterator channel_it = channels.begin(); channel_it != channels.end(); ++channel_it)
        {
            message_kick(*channel_it, *client_it, message, sender);
            (*channel_it)->removeClient(*client_it);
        }
    }
}


void parsing_kick(Command *command, Client *client, Server *server, std::vector<std::string> params)
{
    //print parameters
    int index = 0;
    for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it)
    {
        std::cout << "[" << index << "]" << *it << std::endl;
        index++;
    }

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

    // checking the client
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

    // 2. check if the client is in the channel
    for (std::vector<Channel*>::iterator it_channel = channels.begin(); it_channel != channels.end(); ++it_channel)
    {
        for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
        {
            if (!(*it_channel)->isClientInChannel(*it))
                return client->sendReply(ERR_NOTONCHANNEL((*it)->getNickname()));
        }
    }
    execute_kick(channels, clients, command, client);
}

void KICK(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();
    std::vector<std::string> params = command->getParameters();

    parsing_kick(command, client, server, params);
}

