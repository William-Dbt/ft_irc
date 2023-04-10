#include "Command.hpp"

void AWAY(Command* command)
{
	Client* client = command->getClient();
	std::string message = command->getEndParam();

	if (message.empty() && client->isModeInUse('a'))
	{
		client->removeMode('a');
		client->setAwayMessage("");
		client->sendReply(RPL_UNAWAY());
	}
	else if (!message.empty())
	{
		client->addMode('a');
		client->setAwayMessage(message);
		client->sendReply(RPL_NOWAWAY());
	}

}