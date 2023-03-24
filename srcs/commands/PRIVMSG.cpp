#include "Command.hpp"

void PRIVMSG(Command *command)
{
	Client		*senderClient = command->getClient();
	Server		*server = senderClient->getServer();
	std::string	message = command->getEndParam();
	std::string	target = command->getParameters()[1];
	std::string	sender = command->getClient()->getNickname();

	std::cout << "PRIVMSG: " << message << std::endl;
	std::cout << "Target: " << target << std::endl;
	std::cout << "Sender: " << sender << std::endl;

	if (target[0] == '#')
	{
		Channel *channel = server->getChannel(target);
		if (channel == NULL)
			return senderClient->sendReply(ERR_NOSUCHCHANNEL(target));
		if (!channel->isClientInChannel(senderClient))
			return senderClient->sendReply(ERR_NOTONCHANNEL(target));
		// channel->sendToAllClients(":" + sender + "!" + client->getUsername() + "@" + client->getHostname() + " PRIVMSG " + target + " :" + message);
	}
	else
	{
		Client *targetClient = server->getClient(target);
		if (targetClient == NULL)
			return senderClient->sendReply(ERR_NOSUCHNICK(target));
		targetClient->send(":" + senderClient->getPrefix() + " PRIVMSG " + target + " :" + message);
	}


}
