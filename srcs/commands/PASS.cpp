#include "Command.hpp"

void	PASS(Command* command) {
	Client*	client = command->getClient();

	if (client->status == CONNECTED || client->status == REGISTER)
		return client->sendReply(ERR_ALREADYREGISTRED());

	if (command->getValues().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getValues()[0]));

	if (client->getServer()->getPassword().compare(command->getValues()[1]) != 0)
		client->status = BADPASSWORD;

	client->status = REGISTER;
}
