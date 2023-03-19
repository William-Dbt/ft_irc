#include <fstream>
#include "Command.hpp"

void	MOTD(Command* command) {
	std::fstream	file;
	std::string		motdFilePath;
	char			buffer[254];
	Client*			client = command->getClient();

	motdFilePath = client->getServer()->getConfig().get("motd_file");
	if (motdFilePath.empty())
		return client->sendReply(ERR_NOMOTD());

	file.open(motdFilePath.c_str(), std::ifstream::in);
	if (!file.is_open())
		return client->sendReply(ERR_NOMOTD());

	client->sendReply(RPL_MOTDSTART(client->getServer()->getConfig().get("server_name")));
	while (file.getline(buffer, sizeof(buffer)))
		client->sendReply(RPL_MOTD(std::string(buffer)));

	client->sendReply(RPL_ENDOFMOTD());
}
