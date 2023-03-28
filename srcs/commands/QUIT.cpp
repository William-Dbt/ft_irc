#include "Command.hpp"

void	msgForChannels(Server* server, Client* client)
{
	std::vector<Channel *>				channelsWhereClientIs;
	std::vector<Channel >	channels;

	channels = server->getChannels();	
	for (std::vector<Channel >::iterator it = channels.begin(); it != channels.end(); it++)
		if ((*it).isClientInChannel(client))
			channelsWhereClientIs.push_back(&(*it));

	while (channelsWhereClientIs.size() > 0)
	{
		channelsWhereClientIs[0]->sendToAllClients(":" + client->getPrefix() + " QUIT :" + client->getQuitMessage());
		channelsWhereClientIs.erase(channelsWhereClientIs.begin());
	}
}

void	QUIT(Command* command) {

	Server*		server = command->getServer();
	Client*		client = command->getClient();

	command->getClient()->setQuitMessage(command->getEndParam());
	msgForChannels(server, client);
	command->getClient()->status = DISCONNECTED;
}
