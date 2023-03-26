#include "Command.hpp"

void message_part(Channel *channel, Client *clientToKick, std::string message)
{
    std::map<int, Client *> clients = channel->getClients();
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
        (*it).second->sendTo("PART " + channel->getName() + " " + clientToKick->getNickname() + " " + message);
}

void execute_part(std::vector<std::string> channels_name, Client *client, Command *command)
{
    std::string message = command->getEndParam();
    for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); ++it)
    {
        Channel *channel = client->getServer()->getChannel(*it);
        message_part(channel, client, message);
        channel->removeClient(client);
    }
}

void parsing_part(Command *command, Client *client, Server *server, std::vector<std::string> params)
{
    // check if the command has enough params
    if (params.size() < 2)
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
    execute_part(channels_name, client, command);
}

void PART(Command *command)
{
    Client *client = command->getClient();
    Server *server = client->getServer();
    std::vector<std::string> params = command->getParameters();

    //print params
    for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
    std::cout << std::endl;

    parsing_part(command, client, server, params);
}
