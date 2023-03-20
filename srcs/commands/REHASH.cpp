#include "Command.hpp"

void	REHASH(Command* command) {
	Client*	client = command->getClient();

	if (!client->isModeInUse('o'))
		return client->sendReply(ERR_NOPRIVILEGES());

	command->getServer()->getConfig().setFileConfig();
	return client->sendReply(RPL_REHASHING(command->getServer()->getConfig().get("config_file")));
}
