#include "Command.hpp"

void	INVITE(Command *command)
{
	Client		*senderClient = command->getClient();
	Server		*server = senderClient->getServer();
	std::string	target = command->getParameters()[1];
	std::string	channel = command->getParameters()[2];

	if (target[0] == '#')
		return senderClient->sendReply(ERR_NOSUCHNICK(target));
	Client *targetClient = server->getClient(target);
	if (targetClient == NULL)
		return senderClient->sendReply(ERR_NOSUCHNICK(target));
	// if (targetClient->getMode('i') && !targetClient->isClientInChannel(server->getChannel(channel)))
		// return senderClient->sendReply(ERR_USERNOTINCHANNEL(target, channel));
	targetClient->send(":" + senderClient->getPrefix() + " INVITE " + target + " :" + channel);
}