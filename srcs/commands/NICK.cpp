#include "Command.hpp"

/*
 # ERR_NONICKNAMEGIVEN: No nickname given
 # ERR_RESTRICTED: Mode +r is set
 # ERR_ERRONEUSNICKNAME: Forbidden characters in nickname
 # ERR_NICKNAMEINUSE: Nickname is already used by another user
 #
 # Check to do this part (maybe unnecessary)
 # ERR_UNAVAILRESOURCE: Refers to the nick delay mechanism
 # ERR_NICKCOLLISION: Refers to a nickname already inuse by another server
*/

// nickname   =  ( letter / special ) *8( letter / digit / special / "-" )

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

	if (client->status == COMMING) { // In case of no password given
		client->status = BADPASSWORD;
		return ;
	}
	if (command->getValues().size() < 2 || command->getValues()[1].empty())
		return client->sendReply(ERR_NONICKNAMEGIVEN());

	if (client->isModeInUse('r'))
		return client->sendReply(ERR_RESTRICTED());

	nickname = command->getValues()[1];
	if (!isNicknameProperlyFormatted(nickname))
		return client->sendReply(ERR_ERRONEUSNICKNAME(nickname));

	if (isNicknameAlreadyInUse(client->getServer(), nickname))
		return client->sendReply(ERR_NICKNAMEINUSE(nickname));

	client->sendTo("NICK :" + nickname);
	client->setNickname(nickname);
}