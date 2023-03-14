#include "Command.hpp"

void	PING(Command* command) {
	std::cout << command->getLine() << std::endl;
}
