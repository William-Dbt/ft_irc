#include "Command.hpp"

void	NICK(Command* command) {
	command->getClient()->setNickname(command->getValues()[1]);
}
