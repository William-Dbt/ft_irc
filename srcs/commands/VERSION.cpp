#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"

void	VERSION(Command* command) {
	Client* client = command->getClient();
	Server* server = client->getServer();


	client->sendReply(RPL_VERSION(server->getConfig().get("version"),
						"1",
						server->getConfig().get("server_name"),
						""));
}

