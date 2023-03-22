#include "Command.hpp"

std::vector<std::string> splitCommand(std::string str, char c)
{
	std::vector<std::string> result;
	std::string tmp;

	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == c)
		{
			result.push_back(tmp);
			tmp = "";
		}
		else
		{
			tmp += *it;
		}
	}
	result.push_back(tmp);

	return result;
}

bool isKeyCorrect(Channel *channel, std::string key)
{
	if (channel->getKey().empty())
		return true;
	if (channel->getKey() == key)
		return true;
	return false;
}

void leaveAllChannels(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();

	std::vector<Channel > channels = server->getChannels();
	for (std::vector<Channel >::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		(*it).removeClient(client);
		if ((*it).getClients().size() == 0)
			server->deleteChannel((*it).getName());
		std::cout << "Client " << client->getNickname() << " left channel " << (*it).getName() << std::endl;
	}
	std::cout << "Client " << client->getNickname() << " left all channels" << std::endl;
}

void JOIN(Command *command)
{
	Client *client = command->getClient();
	Server *server = client->getServer();

	if (command->getParameters().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));
	if (command->getParameters()[1] == "0")
		return leaveAllChannels(command);

	std::vector<std::string> channelsNames = splitCommand(command->getParameters()[1], ',');
	std::vector<std::string> channelsKeys;
	if (command->getParameters().size() > 2)
		channelsKeys = splitCommand(command->getParameters()[2], ',');

	for (std::vector<std::string>::iterator it = channelsNames.begin(); it != channelsNames.end(); it++)
	{
		if ((*it)[0] != '#')
			return client->sendReply(ERR_BADCHANMASK(*it));

		Channel *channel = server->getChannel(*it);
		if (channel == NULL)
		{
			server->addChannel(*it);
			channel = server->getChannel(*it);
			if (channelsKeys.size())
			{
				if (channelsKeys[0] != "x")
					channel->setKey(channelsKeys[0]);
				channelsKeys.erase(channelsKeys.begin());
			}
			channel->addClient(client);
		}
		else
		{
			std::string Key;
			if (channelsKeys.size())
			{
				Key = channelsKeys[0];
				channelsKeys.erase(channelsKeys.begin());
			}
			else
				Key = "";

			if (isKeyCorrect(channel, Key))
				channel->addClient(client);
			else
				return client->sendReply(ERR_BADCHANNELKEY(*it));
		}
	}
}
