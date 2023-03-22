#include "Command.hpp"

void	OPER(Command* command) {
	std::string	modes;
	Client*		client = command->getClient();

	if (command->getParameters().size() != 3)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getParameters()[0]));

	if (command->getParameters()[1] != client->getServer()->getConfig().get("oper_username"))
		return client->sendReply(ERR_PASSWDMISMATCH());

	if (command->getParameters()[2] != client->getServer()->getConfig().get("oper_password"))
		return client->sendReply(ERR_PASSWDMISMATCH());

	client->sendReply(RPL_YOUREOPER());
	if (!client->isModeInUse('o')) {
		client->addMode('o');
		if (client->getUserModes().size()) {
			modes = "+";
			modes += client->getUserModes();
		}
		client->sendReply(RPL_UMODEIS(modes));
	}
}
