#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "Server.hpp"

bool	serverRunning = true;

void	signalsHandler(int signum) {
	(void)signum;
	serverRunning = false;
}

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

	signal(SIGINT, signalsHandler);
	while (serverRunning)
		server.run();

	return 0;
}
