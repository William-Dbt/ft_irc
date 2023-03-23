#include "Command.hpp"

void TOPIC(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();

	if (command->getParameters().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	Channel *channel = server->getChannel(command->getParameters()[1]);
	if (channel == NULL)
		return client->sendReply(ERR_NOSUCHCHANNEL(command->getParameters()[1]));
	if (!channel->isClientInChannel(client))
		return client->sendReply(ERR_NOTONCHANNEL(command->getParameters()[1]));

	if (command->getParameters().size() < 3)
		return client->sendReply(RPL_TOPIC(channel->getName(), channel->getTopic()));
	else
	{
		std::string topic = command->getParameters()[2];
		topic.erase(topic.begin());
		channel->setTopic(topic);
		return client->sendReply(RPL_TOPIC(channel->getName(), channel->getTopic()));
	}
}
