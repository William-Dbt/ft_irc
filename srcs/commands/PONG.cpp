#include "Command.hpp"
#include "Client.hpp"

void	PONG(Command* command) {
	Client*	client = command->getClient();

	if (command->getParameters().size() < 2) {
		client->sendReply(ERR_NOORIGIN());
		return ;
	}

	client->setLastPing(time(NULL));
}