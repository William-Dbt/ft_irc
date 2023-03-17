#include "Command.hpp"

std::vector<std::string>	split(std::string str, char c) {
	std::vector<std::string>	result;
	std::string					tmp;

	for (std::string::iterator it = str.begin(); it != str.end(); it++) {
		if (*it == c) {
			result.push_back(tmp);
			tmp = "";
		} else {
			tmp += *it;
		}
	}
	result.push_back(tmp);

	return result;
}

void	JOIN(Command* command) {
	Client*	client = command->getClient();
	Server*	server = client->getServer();

	if (command->getValues().size() < 2)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getValues()[0]));
	if (command->getValues()[1] == "0") {
		// for (std::vector<Channel*>::iterator it = command->getClient()->getChannels().begin(); it != command->getClient()->getChannels().end(); it++) {
		// 	(*it)->removeClient(command->getClient());
		// 	command->getClient()->removeChannel(*it);
		// }
		std::cout << "Client want to leave all channels" << std::endl;
		return ;
	}

	std::vector<std::string>	channelsNames = split(command->getValues()[1], ',');
	std::vector<std::string>	channelsKeys = split(command->getValues()[2], ',');

	for (std::vector<std::string>::iterator it = channelsNames.begin(); it != channelsNames.end(); it++) {
		Channel*	channel = server->getChannel(*it);
		if (channel == NULL) {
			channel = new Channel(*it);
			server->addChannel(channel);
		}
		channel->addClient(client);
		// client->addChannel(channel);
	}
}






















/* 

	Syntax:

JOIN [-window] [-invite] [-<server tag>] <channels> [<keys>]

Parameters:

    -window          Joins a channel in the active window.
    -invite          Joins the channel you were last invited to.
    -<server tag>    The server tag on which you want to join the channel.

    The channel names, separated by a comma, to join and the channel key.

Description:

    Joins the given channels.

Examples:

    /JOIN #irssi
    /JOIN #basementcat secret_lair
    /JOIN -invite
    /JOIN -freenode #github,#freenode,#irssi

 */