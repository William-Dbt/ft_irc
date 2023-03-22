#include "Command.hpp"
#include "Client.hpp"

void	PING(Command* command) {
	Client*	client = command->getClient();

	if (command->getParameters().size() < 2)
		return client->sendReply(ERR_NOORIGIN());

	client->sendTo("PONG :" + command->getParameters()[1]);
}
