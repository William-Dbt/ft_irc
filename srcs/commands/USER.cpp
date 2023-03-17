#include "Command.hpp"

void	USER(Command* command) {
	Client*	client = command->getClient();

	if (command->getValues().size() < 5)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getValues()[0]));

	if (client->status != REGISTER)
		return client->sendReply(ERR_ALREADYREGISTRED());

	client->setUsername(command->getValues()[1]);
	client->setRealname(command->getValues()[4].substr(1, command->getValues()[4].size()));
	client->status = FULLYREGISTER;
}
