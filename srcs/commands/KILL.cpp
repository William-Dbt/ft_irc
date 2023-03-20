#include "Command.hpp"

void	KILL(Command* command) {
	Client*		client = command->getClient();
	std::string	buffer;

	if (!client->isModeInUse('o'))
		return client->sendReply(ERR_NOPRIVILEGES());

	if (command->getValues().size() < 3 || command->getValues()[2] == ":")
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getValues()[0]));

	Client*	victim = command->getServer()->getClient(command->getValues()[1]);
	if (!victim)
		return client->sendReply(ERR_NOSUCHNICK(command->getValues()[1]));

	size_t	posStartComment = command->getLine().find(':') + 1;

	buffer = command->getLine().substr(posStartComment);
	victim->setQuitMessage("<KILLED> " + buffer);
	victim->status = DISCONNECTED;
	victim->sendTo("KILL :" + buffer);
}
