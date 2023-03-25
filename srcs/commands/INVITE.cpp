#include "Command.hpp"

void	INVITE(Command *command)
{
	Server		*server;
	Channel		*channel;
	Client		*senderClient;
	Client		*targetClient;
	std::string	nicknameStr;
	std::string	channelStr;

	senderClient = command->getClient();
	server = senderClient->getServer();

	// ERR_NEEDMOREPARAMS if no parameters are given
	if (command->getParameters().size() < 3)
		return senderClient->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	// ERR_NOSUCHNICK if the sender is not registered
	nicknameStr = command->getParameters()[1];
	if (server->getClient(nicknameStr) == NULL)
		return senderClient->sendReply(ERR_NOSUCHNICK(nicknameStr));

	channelStr = command->getParameters()[2];
	if (server->getChannel(channelStr))
	{
		channel = server->getChannel(channelStr);

		// ERR_NOTONCHANNEL if the sender is not on the channel
		if (!channel->isClientInChannel(senderClient))
			return senderClient->sendReply(ERR_NOTONCHANNEL(channelStr));
		
		// ERR_USERONCHANNEL if the target is already on the channel
		if (channel->isClientInChannel(server->getClient(nicknameStr)))
			return senderClient->sendReply(ERR_USERONCHANNEL(targetClient->getUsername(), channelStr));
	}

	// ERR_CHANOPRIVSNEEDED if the sender is not a channel operator
	// if (!channel->isClientOperator(senderClient))
	// 	return senderClient->sendReply(ERR_CHANOPRIVSNEEDED(channelStr));

	targetClient = server->getClient(nicknameStr);
	targetClient->sendFrom(senderClient, "INVITE " + nicknameStr + " " + channelStr);
	senderClient->sendReply(RPL_INVITING(channelStr, nicknameStr));
}

/*
INVITE <nickname> <channel>

INVITE Wiz #Twilight_Zon
	Command to invite WiZ to #Twilight_zone

:Angel!wings@irc.org INVITE Wiz #Dust
	Message to WiZ when he has been invited by user Angel to channel #Dust



ERR_NEEDMOREPARAMS =	Returned by the server by numerous commands to indicate to
						the client that it didn't supply enough parameters.
							"461" + command + " :Not enough parameters"
ERR_NOSUCHNICK =	Used to indicate the nickname parameter supplied to a command
					is currently unused.
						"401" + nickname + " :No such nick/channel"
ERR_NOTONCHANNEL =	Returned by the server whenever a client tries to perform a
					channel affecting command for which the client isn't a member.
						"442" + channel + " :You're not on that channel"
ERR_USERONCHANNEL =	Returned when a client tries to invite a user to a
					channel they are already on.
						"443" + nickname + channel + " :is already on channel"
ERR_CHANOPRIVSNEEDED =	Any command requiring 'chanop' privileges (such as
						MODE messages) MUST return this error if the client
						making the attempt is not a chanop on the specified
						channel.
							"482" + channel + " :You're not channel operator"

RPL_INVITING =	Returned by the server to indicate that the
				attempted INVITE message was successful and is
				being passed onto the end client.
					"301" + nick + " :" + message

RPL_AWAY =	These replies are used with the AWAY command (if
			allowed).  RPL_AWAY is sent to any client sending a
			PRIVMSG to a client which is away.  RPL_AWAY is only
			sent by the server to which the client is connected.
			Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the
			client removes and sets an AWAY message.
				 "<nick> :<away message>"


*/