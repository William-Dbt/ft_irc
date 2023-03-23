#include "Command.hpp"
#include "Channel.hpp"

void KICK(Command* command) {
    Client* client = command->getClient();
    Server* server = command->getServer();
    std::vector<std::string> params = command->getParameters();
    std::string channelName = params[0];
    std::string nick = params[1];
    std::string comment = params[2];
    Channel* channel = server->getChannel(channelName);
    Client* target = server->getClient(nick);
    if (channel == NULL) {
        return client->sendReply(ERR_NOSUCHCHANNEL(channelName));
    }
    if (target == NULL) {
        return client->sendReply(ERR_NOSUCHNICK(nick));
    }
    if (channel->getMode('o') == false) {
        return client->sendReply(ERR_CHANOPRIVSNEEDED(channelName));
    }
    if (target->getChannel(channelName) == NULL) {
        return client->sendReply(ERR_USERNOTINCHANNEL(nick, channelName));
    }
    channel->removeClient(target);
    target->removeChannel(channelName);
    channel->sendToChannel(":" + client->getNickname() + " KICK " + channelName + " " + nick + " :" + comment);
}