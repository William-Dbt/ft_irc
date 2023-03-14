#include "Command.hpp"

void	QUIT(Command* command) {
	std::cout << command->getLine() << std::endl;
}
