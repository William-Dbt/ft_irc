#include "Command.hpp"

void	USER(Command* command) {
	Client*	client = command->getClient();

	if (command->getParameters().size() < 5)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	if (client->status != REGISTER && client->status != BADNICKNAME)
		return client->sendReply(ERR_ALREADYREGISTRED());

	client->setUsername(command->getParameters()[1]);
	client->setRealname(command->getParameters()[4].substr(1, command->getParameters()[4].size()));
	if (client->status != BADNICKNAME)
		client->status = FULLYREGISTER;
}
