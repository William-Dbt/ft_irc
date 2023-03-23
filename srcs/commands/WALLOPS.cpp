#include "Command.hpp"

void	WALLOPS(Command* command) {
	Client*	client = command->getClient();

	if (command->getParameters().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	if (!client->isModeInUse('o'))
		return ;

	std::map<int, Client*>::iterator	it;

	for (it = command->getServer()->getClients().begin(); it != command->getServer()->getClients().end(); it++) {
		if ((*it).second->isModeInUse('w'))
			(*it).second->sendTo("WALLOPS :" + command->getEndParam());
	}
}
