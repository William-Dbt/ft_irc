#include "Command.hpp"

extern bool	g_bServerRunning;

void	DIE(Command* command) {
	Client*	client = command->getClient();

	if (!client->isModeInUse('o'))
		return client->sendReply(ERR_NOPRIVILEGES());

	std::map<int, Client*>::iterator	it;

	for (it = command->getServer()->getClients().begin(); it != command->getServer()->getClients().end(); it++) {
		(*it).second->setQuitMessage("Server disconnected");
		(*it).second->status = DISCONNECTED;
	}
	g_bServerRunning = false;
}
