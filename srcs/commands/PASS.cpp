#include "Command.hpp"

void	PASS(Command* command) {
	Client*	client = command->getClient();

	if (client->status == CONNECTED || client->status == REGISTER)
		return client->sendReply(ERR_ALREADYREGISTRED());

	if (command->getParameters().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	if (client->getServer()->getPassword().compare(command->getParameters()[1]) != 0) {
		client->status = BADPASSWORD;
		return ;
	}
	client->status = REGISTER;
}
