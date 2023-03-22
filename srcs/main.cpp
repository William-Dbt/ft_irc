#include <iostream>
#include <cstdlib>
#include <signal.h>
#include "Server.hpp"

bool	g_bServerRunning = true;

void	signalsHandler(int signum) {
	(void)signum;
	g_bServerRunning = false;
	std::cout << "\b\b\b";
	printLog("Shutdown server.", SERVER);
}

int	main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	printLog("Server is starting...", SERVER);
	
	int		exitStatus;
	Server	server(atoi(argv[1]), argv[2]);

	exitStatus = server.init();
	if (exitStatus)
		return exitStatus;

	signal(SIGINT, signalsHandler);
	while (g_bServerRunning)
		server.run();

	return 0;
}
