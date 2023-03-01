#include <iostream>
#include <cstdlib>
#include "Server.hpp"

int	main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	
	int		exitStatus;
	Server	server(atoi(argv[1]), argv[2]);

	exitStatus = server.init();
	if (exitStatus)
		return exitStatus;

	while (server.running)
		server.run();

	return 0;
}
