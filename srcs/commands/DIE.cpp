#include "Command.hpp"

extern bool	g_bServerRunning;

void	DIE(Command* command) {
	Client*	client = command->getClient();

	if (!client->isModeInUse('o'))
		return client->sendReply(ERR_NOPRIVILEGES());

	g_bServerRunning = false;
}
