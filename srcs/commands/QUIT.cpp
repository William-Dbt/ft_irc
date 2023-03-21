#include "Command.hpp"

void	QUIT(Command* command) {
	std::string	buffer;

	buffer = command->getLine().substr(6, command->getLine().size() - 6 - 2); // -2 refers to /r/n
	if (!buffer.empty())
		command->getClient()->setQuitMessage(buffer);

	command->getClient()->status = DISCONNECTED;
}
