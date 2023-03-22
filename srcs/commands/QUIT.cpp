#include "Command.hpp"

void	QUIT(Command* command) {
	command->getClient()->setQuitMessage(command->getEndParam());
	command->getClient()->status = DISCONNECTED;
}
