#include "Command.hpp"
#include "Channel.hpp"

void KICK(Command *command)
{
    Client *client = command->getClient();
    Server *server = client->getServer();
    std::vector<std::string> params = command->getParameters();

    // check if the command has enough params
    if (params.size() < 4)
        return client->sendReply(ERR_NEEDMOREPARAMS(params[0]));

    // check if the channel name is valid -> mask = #
    if (params[1][0] != '#')
        return client->sendReply(ERR_BADCHANMASK(params[1]));

    // check if the channel exists
    // vector of channels
    std::vector<std::string> channels_name = command->multipleParams(params[1]);
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

    //  2. check if the nickname is in the channel
    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        for (std::vector<Client*>::iterator it2 = clients.begin(); it2 != clients.end(); ++it2)
        {
            if (!(*it)->isClientInChannel(*it2))
                return client->sendReply(ERR_NOTONCHANNEL((*it)->getName()));
        }
    }

    for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        for (std::vector<Client*>::iterator it2 = clients.begin(); it2 != clients.end(); ++it2)
                (*it)->removeClient(*it2);
    }    
}
