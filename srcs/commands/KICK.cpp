#include "Command.hpp"
#include "Channel.hpp"



void KICK(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();
    std::vector<std::string> params = command->getParameters();

    // std::cout << "KICK command received from " << client->getNickname() << std::endl;
    // std::cout << "KICK params size = " << params.size() << std::endl;
    // for (size_t i = 0; i < params.size(); i++)
    // {
    //     std::cout << "KICK params[" << i << "] = " << params[i] << std::endl;
    // }

    // check if the command has enough params
    if (params.size() < 4)
        return client->sendReply(ERR_NEEDMOREPARAMS(params[0]));
    
    // check if the channel name is valid -> mask = #
    if (params[1][0] != '#')
			return client->sendReply(ERR_BADCHANMASK(params[1]));

    // check if the channel exists
	Channel *channel = server->getChannel(command->getParameters()[1]);
	if (channel == NULL)
		return client->sendReply(ERR_NOSUCHCHANNEL(command->getParameters()[1]));

    // check if params[2] is a valid nickname
    //  1. check if the nickname is in the server
    std::cout << "KICK start"<< std::endl;
    Client *clientToKick = server->getClient(params[2]);
    if (!clientToKick)
    {
        std::cout << "KICK: clientToKick is NULL" << std::endl;
        return client->sendReply(ERR_NOTONCHANNEL(command->getParameters()[2]));
    }
    std::cout << "KICK end"<< std::endl;

    //  2. check if the nickname is in the channel
	if (!channel->isClientInChannel(clientToKick))
		return client->sendReply(ERR_NOTONCHANNEL(command->getParameters()[2]));

    
    channel->removeClient(clientToKick);
}
