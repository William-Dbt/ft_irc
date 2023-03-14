#include "Command.hpp"

void	USER(Command* command) {
	size_t	pos;

	pos = command->getLine().find(' ');
	command->getClient()->setUsername(command->getLine().substr(pos + 1, command->getLine().find(' ', pos + 1) - 5)); // (-5 refers to the string "USER " before the command)
	command->getClient()->setRealname(command->getLine().substr(command->getLine().find(':') + 1, command->getLine().size()));
	if (command->getClient()->status != REGISTER && command->getClient()->status != BADPASSWORD)
		command->getClient()->status = REGISTER;
}
