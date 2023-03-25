#include "Command.hpp"

void execute_part(std::vector<std::string> channels_name, Client *client)
{
    for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); ++it)
    {
        Channel *channel = client->getServer()->getChannel(*it);
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
    execute_part(channels_name, client);
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
