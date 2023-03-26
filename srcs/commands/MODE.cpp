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

void	MODE(Command* command) {
	bool					addMode = true;
	std::string				modes;
	std::string::iterator	it;
	Client*					client = command->getClient();

	if (command->getParameters().size() == 1)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	if (command->getParameters()[1][0] == '#')
		return client->send("Channel mods aren't supported by this server.");

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
