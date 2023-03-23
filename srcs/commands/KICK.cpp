#include "Command.hpp"
#include "Channel.hpp"

void KICK(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();

    // Check if the client has enough parameters
    // 1 -> channel name
    // 2 -> target nick
	if (command->getParameters().size() < 3)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

    // check if the channel exists and if the client is in it
	Channel *channel = server->getChannel(command->getParameters()[1]);
	if (channel == NULL)
		return client->sendReply(ERR_NOSUCHCHANNEL(command->getParameters()[1]));
	if (!channel->isClientInChannel(client))
		return client->sendReply(ERR_NOTONCHANNEL(command->getParameters()[1]));

    // if parameter[3] is a reason, we add it to the kick message
    // ? still to know how to write on chanel and how eject client ?
    if (command->getParameters().size() > 3)
    {
        std::string reason = command->getParameters()[3];
        // server->sendToChannel(channel, ":" + client->getPrefix() + " KICK " + channel->getName() + " " + command->getParameters()[2] + " :" + reason);
    }
    // else
    //     server->sendToChannel(channel, ":" + client->getPrefix() + " KICK " + channel->getName() + " " + command->getParameters()[2] + " :Kicked out of channel");

}
