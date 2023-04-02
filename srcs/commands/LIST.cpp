#include "Command.hpp"
#include <sstream>

void LIST(Command *command)
{
	std::vector<std::string>	channels;
	Channel*					channel;

	if (command->getParameters().size() == 3)
		return command->getClient()->sendReply(ERR_NOSUCHSERVER(command->getParameters()[2]));

	if (command->getParameters().size() == 2)
	{
		if (command->getParameters()[1] == "*")
			;
		else
			channels = command->multipleParams(command->getParameters()[1]);
	}
	if (channels.size() == 0)
	{
		channels = command->getServer()->getChannelsNames();
		while (channels.size() > 0)
		{
			channel = command->getServer()->getChannel(channels[0]);

			int numVisible = channel->getClients().size();
			std::ostringstream ssNumVisible;
			ssNumVisible << numVisible;

			command->getClient()->sendReply(RPL_LIST(channels[0], ssNumVisible.str(), channel->getTopic()));
			channels.erase(channels.begin());
		}
	}
	else
	{
		while (channels.size() > 0)
		{
			channel = command->getServer()->getChannel(channels[0]);
			if (!channel)
			{
				channels.erase(channels.begin());
				continue ;
			}

			int numVisible = channel->getClients().size();
			std::ostringstream ssNumVisible;
			ssNumVisible << numVisible;

			command->getClient()->sendReply(RPL_LIST(channels[0], ssNumVisible.str(), channel->getTopic()));
			channels.erase(channels.begin());
		}
	}
	command->getClient()->sendReply(RPL_LISTEND());
}







/*
Command: LIST
Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

ERR_TOOMANYMATCHES

ERR_NOSUCHSERVER =	Used to indicate the server name given currently does not exist.
							"402" + servername + " :No such server"	



					Replies RPL_LIST, RPL_LISTEND mark the actual replies
					with data and end of the server's response to a LIST
					command.  If there are no channels available to return,
					only the end reply MUST be sent.
RPL_LIST = "<channel> <# visible> :<topic>"
RPL_LISTEND





LIST
; Command to list all channels.

LIST #twilight_zone,#42
; Command to list channels #twilight_zone and #42

*/