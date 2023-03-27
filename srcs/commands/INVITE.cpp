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

	targetClient = server->getClient(nicknameStr);
	channelStr = command->getParameters()[2];

	channel = server->getChannel(channelStr);
	if (channel)
	{
		// ERR_NOTONCHANNEL if the sender is not on the channel
		if (!channel->isClientInChannel(senderClient))
			return senderClient->sendReply(ERR_NOTONCHANNEL(channelStr));
		
		// ERR_USERONCHANNEL if the target is already on the channel
		if (channel->isClientInChannel(server->getClient(nicknameStr)))
			return senderClient->sendReply(ERR_USERONCHANNEL(targetClient->getUsername(), channelStr));
	}
	// ERR_CHANOPRIVSNEEDED if the sender is not a channel operator
	if (channel->getInviteStatus() == true && !channel->isClientOperator(senderClient))
		return senderClient->sendReply(ERR_CHANOPRIVSNEEDED(channelStr));

	if (targetClient->isModeInUse('a'))
		senderClient->sendReply(RPL_AWAY(nicknameStr, targetClient->getAwayMessage()));

	channel->addInvitedClient(targetClient);
	targetClient->sendFrom(senderClient, "INVITE " + nicknameStr + " " + channelStr);
	senderClient->sendReply(RPL_INVITING(channelStr, nicknameStr));
}
