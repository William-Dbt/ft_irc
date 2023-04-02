#include "Command.hpp"

static bool	isNicknameProperlyFormatted(std::string nickname) {
	size_t	i = 0;

	if (nickname.size() > 9)
		return false;

	while (i < nickname.size()) {
		if (i < 8) {
			if (!isLetter(nickname[i]) && !isSpecial(nickname[i]))
				return false;
		}
		else
			if (!isLetter(nickname[i]) && !isDigit(nickname[i])
				&& !isSpecial(nickname[i]) && nickname[i] != '-')
				return false;

		i++;
	}
	return true;
}

static bool	isNicknameAlreadyInUse(Server* server, std::string nickname) {
	std::map<int, Client*>::iterator	it;

	for (it = server->getClients().begin(); it != server->getClients().end(); it++)
		if ((*it).second->getNickname().compare(nickname) == 0)
			return true;

	return false;
}

void	NICK(Command* command) {
	std::string	nickname;
	Client*		client = command->getClient();

	if (client->status == COMMING) { // The user hasn't give a password
		if (!command->getServer()->getPassword().empty()) {
			client->status = BADPASSWORD;
			return ;
		}
		else
			client->status = REGISTER;
	}
	if (command->getParameters().size() < 2 || command->getParameters()[1].empty())
		return client->sendReply(ERR_NONICKNAMEGIVEN());

	if (client->isModeInUse('r'))
		return client->sendReply(ERR_RESTRICTED());

	nickname = command->getParameters()[1];
	if (!isNicknameProperlyFormatted(nickname))
		return client->sendReply(ERR_ERRONEUSNICKNAME(nickname));

	if (isNicknameAlreadyInUse(client->getServer(), nickname)) {
		client->status = BADNICKNAME;
		return client->sendReply(ERR_NICKNAMEINUSE(nickname));
	}
	client->sendTo("NICK :" + nickname);
	client->setNickname(nickname);
	if (client->status == BADNICKNAME)
		client->status = FULLYREGISTER;
}
