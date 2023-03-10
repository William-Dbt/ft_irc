#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "Server.hpp"

bool	g_bServerRunning = true;

void	signalsHandler(int signum) {
	(void)signum;
	g_bServerRunning = false;
	std::cout << "\b\b\b" << KRED << BROADCAST << "Shutdown server." << KRESET << std::endl;
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

	if (DEBUG)
	{
		std::cout << "Message sender by color:" << std::endl;
		std::cout << KCYN << "Server to Client Msg" << KRESET << std::endl;
		std::cout << KMAG << "Client to Server Msg" << KRESET << std::endl
				  << std::endl;
	}

	signal(SIGINT, signalsHandler);
	while (g_bServerRunning)
		server.run();

	return 0;
}
