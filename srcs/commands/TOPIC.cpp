#include "Command.hpp"

void TOPIC(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();
	std::string newTopic = command->getEndParam();

	if (command->getParameters().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	Channel *channel = server->getChannel(command->getParameters()[1]);
	if (!channel || !channel->isClientInChannel(client))
		return client->sendReply(ERR_NOTONCHANNEL(channel->getName()));

	std::map<int, Client *> clients = channel->getClients();
	std::map<int, Client *>::iterator it;

	if (newTopic.size() == 0)
	{
		channel->setTopic("");
		for (it = clients.begin(); it != clients.end(); it++)
		{
			// RPL_NOTOPIC doesn't really work, so I'm using TOPIC with an empty topic
			// (*it).second->sendReply(RPL_NOTOPIC(channel->getName()));
			(*it).second->sendTo("TOPIC " + channel->getName() + " :");
		}
	}
	else
	{
		std::string topic = command->getParameters()[2];
		topic.erase(topic.begin());
		channel->setTopic(topic);
		for (it = clients.begin(); it != clients.end(); it++)
			(*it).second->sendReply(RPL_TOPIC(channel->getName(), channel->getTopic()));
	}
}

/*
Command: TOPIC
Parameters: <channel> [ <topic> ]

ERR_NEEDMOREPARAMS		=	Returned by the server by numerous commands to indicate to
							the client that it didn't supply enough parameters.
								"461" + command + " :Not enough parameters"

ERR_NOTONCHANNEL		=	Returned by the server whenever a client tries to perform a
							channel affecting command for which the client isn't a member.
								"442" + channel + " :You're not on that channel"

ERR_CHANOPRIVSNEEDED	=	Any command requiring 'chanop' privileges (such as
							MODE messages) MUST return this error if the client
							making the attempt is not a chanop on the specified
							channel.
								"482" + channel + " :You're not channel operator"

ERR_NOCHANMODES			=	Any command requiring operator privileges to operate
							MUST return this error to indicate the attempt was
							unsuccessful.
								"477" + channel + " :Channel doesn't support modes"

RPL_NOTOPIC	= "331" + channel + " :No topic is set"
RPL_TOPIC	= "332" + channel + " :" + topic



:WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic
; User Wiz setting the topic.

TOPIC #test :another topic
; Command to set the topic on #test to "another topic".

TOPIC #test :
; Command to clear the topic on #test.

TOPIC #test
; Command to check the topic for #test.


*/