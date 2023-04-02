#include "Command.hpp"
#include "Channel.hpp"

void NOTICE(Command *command)
{
	Client		*senderClient = command->getClient();
	Server		*server = senderClient->getServer();
	std::string	message = command->getEndParam();
	std::string	target = command->getParameters()[1];

	if (message.empty())
		return;

	if (target.empty())
		return;

	if (server->getClient(target) == NULL && server->getChannel(target) == NULL)
		return;

	if (target[0] == '#')
	{
		Channel *channel = server->getChannel(target);
		
		std::map<int, Client *> clients = channel->getClients();
		std::map<int, Client *>::iterator it;

		for (it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it).second != senderClient)
				(*it).second->sendFrom(senderClient, "NOTICE " + target + " :" + message);

		}
	}
	else
	{
		Client *targetClient = server->getClient(target);
		if (targetClient == NULL)
			return ;
		targetClient->sendFrom(senderClient, "NOTICE " + target + " :" + message);
	}
}
