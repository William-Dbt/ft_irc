#include "Command.hpp"

void TOPIC(Command *command)
{
	std::cout << "TOPIC function" << std::endl;

	Client *client = command->getClient();
	Server *server = client->getServer();

	if (command->getValues().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getValues()[0]));			// Not enough parameters

	Channel *channel = server->getChannel(command->getValues()[1]);
	if (channel == NULL)
		return client->sendReply(ERR_NOSUCHCHANNEL(command->getValues()[1]));			// No such channel
	if (!channel->isClientInChannel(client))
		return client->sendReply(ERR_NOTONCHANNEL(command->getValues()[1]));			// You're not on that channel

	if (command->getValues().size() < 3)												// get topic
		return client->sendReply(RPL_TOPIC(channel->getName(), channel->getTopic()));	// <topic>
	else																				// set topic
	{
		std::string topic = command->getValues()[2];
		topic.erase(topic.begin());
		channel->setTopic(topic);
		return client->sendReply(RPL_TOPIC(channel->getName(), channel->getTopic()));	// <topic>
	}
}
