#include "Command.hpp"
#include "Client.hpp"

void	PONG(Command* command) {
	Client*	client = command->getClient();

	if (command->getParameters().size() < 2)
		return client->sendReply(ERR_NOORIGIN());

	client->setLastPing(time(NULL));
}