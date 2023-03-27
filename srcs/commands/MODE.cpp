#include "Command.hpp"

static bool	isValidUserModeParam(std::string param, Config* config) {
	std::string::iterator	it = param.begin();

	if ((*it) == '+' || (*it) == '-')
		it++;

	while (it != param.end()) {
		if (!isValidUserMode((*it), config))
			return false;

		it++;
	}
	return true;
}

static void	MODE_User(Command* command, Client* client) {
	bool					addMode = true;
	std::string				modes;
	std::string::iterator	it;

	if (client->getNickname() != command->getParameters()[1])
		return client->sendReply(ERR_USERSDONTMATCH());

	if (command->getParameters().size() == 2) {
		if (!client->getUserModes().size())
			modes.clear();
		else {
			modes = "+";
			modes += client->getUserModes();
		}
		return client->sendReply(RPL_UMODEIS(modes));
	}
	if (!isValidUserModeParam(command->getParameters()[2], &command->getServer()->getConfig()))
		return client->sendReply(ERR_UMODEUNKNOWNFLAG());

	modes = command->getParameters()[2];
	it = modes.begin();
	if ((*it) == '-' || (*it) == '+') {
		if ((*it) == '-')
			addMode = false;

		it++;
	}
	while (it != modes.end()) {
		if ((*it) == 'a') {
			it++;
			continue ;
		}
		if (!addMode) {
			if ((*it) != 'r')
				client->removeMode((*it));
		}
		else {
			if ((*it) != 'o')
				client->addMode((*it));
		}
		it++;
	}
	if (!client->getUserModes().size())
		modes.clear();
	else {
		modes = "+";
		modes += client->getUserModes();
	}
	client->sendReply(RPL_UMODEIS(modes));
}

static void	MODE_Channel(Command* command, Client* client, Channel* channel) {
	bool					addMode = true;
	std::string				modes;
	std::string::iterator	it;

	if (channel->getOperator() && channel->getOperator()->getFd() == client->getFd() && command->getParameters().size() > 2) {
		modes = command->getParameters()[2];
		it = modes.begin();
		if ((*it) == '-' || (*it) == '+') {
			if ((*it) == '-')
				addMode = false;

			it++;
		}
		while (it != modes.end()) {
			if ((*it) != 'i')
				client->sendReply(ERR_UNKNOWNMODE(charToString((*it)), channel->getName()));
			else {
				if (addMode)
					channel->setInviteStatus(true);
				else
					channel->setInviteStatus(false);
			}
			it++;
		}
	}
	client->sendReply(RPL_CHANNELMODEIS(channel->getName(), (channel->getInviteStatus() == true ? " +i" : ""), ""));
}

void	MODE(Command* command) {
	Client*		client = command->getClient();

	if (command->getParameters().size() == 1)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	if (command->getParameters()[1][0] == '#')
		return MODE_Channel(command, client, command->getServer()->getChannel(command->getParameters()[1]));

	return MODE_User(command, client);
}
