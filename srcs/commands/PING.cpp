#include "Command.hpp"

void	PING(Command* command) {
	std::cout << command->getCommandLine() << std::endl;
}
