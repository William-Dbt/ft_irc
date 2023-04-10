#include "Command.hpp"

void	KILL(Command* command) {
	Client*		client = command->getClient();
	std::string	buffer;

	if (!client->isModeInUse('o'))
		return client->sendReply(ERR_NOPRIVILEGES());

	if (command->getParameters().size() < 3 || command->getParameters()[2] == ":")
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	Client*	victim = command->getServer()->getClient(command->getParameters()[1]);
	if (!victim)
		return client->sendReply(ERR_NOSUCHNICK(command->getParameters()[1]));

	size_t	posStartComment = command->getLine().find(':') + 1;

	buffer = command->getLine().substr(posStartComment, command->getLine().size() - posStartComment - 1);
	victim->setQuitMessage("<KILLED> " + buffer);
	client->getServer()->kickClientFromAllChannelsWithJoin(victim);
	victim->status = DISCONNECTED;
	victim->sendTo("KILL :" + buffer);
}
