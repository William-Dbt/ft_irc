#include "Command.hpp"

std::vector<std::string> splitCommand(std::string str, char c)
{
	std::vector<std::string> result;
	std::string tmp;

	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == c)
		{
			result.push_back(tmp);
			tmp = "";
		}
		else
			tmp += *it;
	}
	result.push_back(tmp);

	return result;
}

bool isKeyCorrect(Channel *channel, std::string key)
{
	if (channel->getKey().empty())
		return true;
	if (channel->getKey() == key)
		return true;
	return false;
}

void leaveAllChannels(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();

	std::vector<Channel > channels = server->getChannels();
	for (std::vector<Channel >::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		(*it).removeClient(client);
		if ((*it).getClients().size() == 0)
			server->deleteChannel((*it).getName());
		std::cout << "Client " << client->getNickname() << " left channel " << (*it).getName() << std::endl;
	}
	std::cout << "Client " << client->getNickname() << " left all channels" << std::endl;
}

void JOIN(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();

	if (command->getParameters().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	if (command->getParameters()[1] == "0")
		return leaveAllChannels(command);

	std::vector<std::string> channelsNames = splitCommand(command->getParameters()[1], ',');
	std::vector<std::string> channelsKeys;
	if (command->getParameters().size() > 2)
		channelsKeys = splitCommand(command->getParameters()[2], ',');

	for (std::vector<std::string>::iterator it = channelsNames.begin(); it != channelsNames.end(); it++)
	{
		if ((*it)[0] != '#')
			return client->sendReply(ERR_BADCHANMASK(*it));

		Channel *channel = server->getChannel(*it);
		if (channel == NULL)
		{
			server->addChannel(*it);
			channel = server->getChannel(*it);
			if (channelsKeys.size())
			{
				if (channelsKeys[0] != "x")
					channel->setKey(channelsKeys[0]);
				channelsKeys.erase(channelsKeys.begin());
			}
			channel->addClient(client);
		}
		else
		{
			std::string Key;
			if (channelsKeys.size())
			{
				Key = channelsKeys[0];
				channelsKeys.erase(channelsKeys.begin());
			}
			else
				Key = "";

			if (isKeyCorrect(channel, Key))
				channel->addClient(client);
			else
				return client->sendReply(ERR_BADCHANNELKEY(*it));
		}
		client->sendReply(RPL_NAMREPLY(*it, channel->getClientsNicknames()));
		client->sendReply(RPL_ENDOFNAMES(*it));
	}
}



/*
ERR_NEEDMOREPARAMS =	Returned by the server by numerous commands to indicate to
						the client that it didn't supply enough parameters.
							"461" + command + " :Not enough parameters"

ERR_NOSUCHCHANNEL		= Used to indicate the given channel name is invalid.
								"403" + channelname + " :No such channel"


						Any command requiring operator privileges to operate
						MUST return this error to indicate the attempt was
						unsuccessful
ERR_CHANNELISFULL			= "471" + channel + " :Cannot join channel (+l)"
ERR_INVITEONLYCHAN			= "473" + channel + " :Cannot join channel (+i)"
ERR_BANNEDFROMCHAN			= "474" + channel + " :Cannot join channel (+b)"
ERR_BADCHANNELKEY			= "475" + channel + " :Cannot join channel (+k)"
ERR_BADCHANMASK				= "476" + channel + " :Bad Channel Mask"

ERR_TOOMANYCHANNELS =	Sent to a user when they have joined the maximum
						number of allowed channels and they try to join
						another channel.
							"405" + channelname + " :You have joined too many channels"

ERR_TOOMANYTARGETS =	Returned to a client which is attempting to send a
						PRIVMSG/NOTICE using the user@host destination format
						and for a user@host which has several occurrences.

						Returned to a client which trying to send a
						PRIVMSG/NOTICE to too many recipients.

						Returned to a client which is attempting to JOIN a safe
						channel using the shortname when there are more than one
						such channel.
							"407" + target + " :Duplicate recipients. No message delivered"

ERR_UNAVAILRESOURCE =	Returned by a server to a user trying to join a channel
						currently blocked by the channel delay mechanism.

						Returned by a server to a user trying to change nickname
						when the desired nickname is blocked by the nick delay
						mechanism.
							"437" + nickchannel + " :Nick/channel is temporarily unavailable"

RPL_TOPIC = "332" + channel + " :" + topic



JOIN #foobar
; Command to join channel #foobar.

JOIN &foo fubar
; Command to join channel &foo using key "fubar".

JOIN #foo,&bar fubar
; Command to join channel #foo using key "fubar" and &bar using no key.

JOIN #foo,#bar fubar,foobar
; Command to join channel #foo using key "fubar", and channel #bar using key "foobar".

JOIN #foo,#bar
; Command to join channels #foo and #bar.

JOIN 0
; Leave all currently joined channels.

:WiZ!jto@tolsun.oulu.fi JOIN #Twilight_zone
; JOIN message from WiZ on channel #Twilight_zone

*/