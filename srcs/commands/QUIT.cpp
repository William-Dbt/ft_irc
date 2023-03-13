#include "Command.hpp"

void	QUIT(Command* command) {
	std::cout << command->getCommandLine() << std::endl;
}
