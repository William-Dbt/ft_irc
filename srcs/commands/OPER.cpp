#include "Command.hpp"

void	OPER(Command* command) {
	std::string	modes;
	Client*		client = command->getClient();

	if (command->getValues().size() != 3)
		return client->sendReply(ERR_NEEDMOREPARAMS(command->getValues()[0]));

	if (command->getValues()[1] != client->getServer()->getConfig().get("oper_username"))
		return client->sendReply(ERR_PASSWDMISMATCH());

	if (command->getValues()[2] != client->getServer()->getConfig().get("oper_password"))
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
