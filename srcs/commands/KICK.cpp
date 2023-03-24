#include "Command.hpp"
#include "Channel.hpp"

void KICK(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();
    std::vector<std::string> params = command->getParameters();

    std::cout << "KICK command received from " << client->getNickname() << std::endl;
    std::cout << "KICK params size = " << params.size() << std::endl;
    //print params
    for (size_t i = 0; i < params.size(); i++)
    {
        std::cout << "KICK params[" << i << "] = " << params[i] << std::endl;
    }
    // Check if the client has enough parameters
    // 1 -> channel name
    // 2 -> target nick
	if (params.size() < 4 || params.at(4) == ":")
    {
        std::cout << "KICK not enough params" << std::endl;
		return client->sendReply(ERR_NEEDMOREPARAMS(params[0]));
    }
    std::cout << "KICK parsing pass" << std::endl;
    //
    
    // check if the channel name is valid -> mask = #
    if (params[1][0] != '#')
			return client->sendReply(ERR_BADCHANMASK(params[1]));

    std::cout << "KICK fin du parsing"<< std::endl;
    // check if the channel exists and if the client is in it
	Channel *channel = server->getChannel(command->getParameters()[1]);
	if (channel == NULL)
		return client->sendReply(ERR_NOSUCHCHANNEL(command->getParameters()[1]));
	// if (!channel->isClientInChannel(client))
	// 	return client->sendReply(ERR_NOTONCHANNEL(command->getParameters()[1]));

    // if parameter[3] is a reason, we add it to the kick message
    // ? still to know how to write on chanel and how eject client ?
    // needed : 
    // -> eject client from channel
    // client->ejectFromChannel(channel);
    // if (command->getParameters().size() > 6)
    // {
        // std::string reason = command->getParameters()[3];
        //return client->sendReply(RPL_KICK(channel->getName(), command->getParameters()[2], reason));
    // }
    // return client->sendReply(RPL_KICK(channel->getName(), command->getParameters()[2], "No reason"));
}
