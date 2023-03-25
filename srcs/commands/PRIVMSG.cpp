#include "Command.hpp"
#include "Channel.hpp"

void PRIVMSG(Command *command)
{
	Client		*senderClient = command->getClient();
	Server		*server = senderClient->getServer();
	std::string	message = command->getEndParam();
	std::string	target = command->getParameters()[1];

	if (target[0] == '#')
	{
		Channel *channel = server->getChannel(target);
		if (channel == NULL)
			return senderClient->sendReply(ERR_NOSUCHCHANNEL(target));
		if (!channel->isClientInChannel(senderClient))
			return senderClient->sendReply(ERR_NOTONCHANNEL(target));
		
		std::map<int, Client *> clients = channel->getClients();
		std::map<int, Client *>::iterator it;

		for (it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it).second != senderClient)
				(*it).second->sendFrom(senderClient, "PRIVMSG " + target + " :" + message);

		}
	}
	else
	{
		Client *targetClient = server->getClient(target);
		if (targetClient == NULL)
			return senderClient->sendReply(ERR_NOSUCHNICK(target));
		targetClient->sendFrom(senderClient, "PRIVMSG " + target + " :" + message);
	}


}
