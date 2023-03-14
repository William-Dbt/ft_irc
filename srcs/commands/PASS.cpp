#include "Command.hpp"

void	PASS(Command* command) {
	Client*	client = command->getClient();

	if (client->status == CONNECTED) {
		client->sendReply(ERR_ALREADYREGISTRED());
		return ;
	}
	if (command->getValues().size() < 2) {
		client->sendReply(ERR_NEEDMOREPARAMS(command->getValues()[0]));
		return ;
	}
	if (client->getServer()->getPassword().compare(command->getValues()[1]) != 0)
		client->status = BADPASSWORD;

	client->setPassword(command->getValues()[1]);
}
