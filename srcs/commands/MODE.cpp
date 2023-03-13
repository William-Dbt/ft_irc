#include "Command.hpp"

void	MODE(Command* command) {
	std::cout << command->getCommandLine() << std::endl;
}
