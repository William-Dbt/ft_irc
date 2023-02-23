#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TIMEOUT_LISTENING	500
#define BUFFER_SIZE			4096

int	main(int argc, char *argv[]) {
	// TODO
	// if (argc != 3) {
	if (argc != 2) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int	serverFd,
		opt;


	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0) {
		std::cerr << "Socket creation error" << std::endl;
		return 1;
	}
	// Set socket to non-blocking mode
	if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		std::cerr << "Set socket option error" << std::endl;
		return 293235;
	}
	if (fcntl(serverFd, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "Fcntl error" << std::endl;
		return 145;
	}

	struct sockaddr_in	address;

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(argv[1]));

	// open socket
	if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		std::cerr << "Error: bind socket failed." << std::endl;
		return 2;
	}

	if (listen(serverFd, address.sin_port) < 0) {
		std::cerr << "Error: listen failed." << std::endl;
		return 3;
	}


	int	i,
		maxConnect;

	struct pollfd		*polls = NULL;

	maxConnect = 1000;

	// Initialization of the pool of address that we will listen for
	polls = new pollfd[maxConnect];
	for (i = 0; i < maxConnect; i++) {
		polls[i].fd = -1;
		polls[i].events = POLLIN;
	}

	// Initialize the server
	polls[0].fd = serverFd;

	int	newSocket;/* ,
		bytes,
		n,
		found,
		nbDone,
		resu; */

	ssize_t				size;
	char				buffer[BUFFER_SIZE + 1];
	socklen_t			cliLenght;
	struct sockaddr_in	newAddress;
	std::string			prefix;
	std::string			command;
	int status = 0;

	while (1) {
		if (poll(&polls[0], maxConnect, TIMEOUT_LISTENING) < 0)
			continue ;

		// Is server receive something?
		if (polls[0].revents == POLLIN) {
			cliLenght = sizeof(newAddress);
			newSocket = accept(serverFd, (struct sockaddr*)&newAddress, &cliLenght);
			if (newSocket == -1)
				continue ;

			i = 0;
			while (i < maxConnect && polls[i].fd != -1)
				i++;

			polls[i].fd = newSocket;
			polls[i].revents = POLLIN;
			std::cout << "----- New client -----" << std::endl;
			std::cout << "IP: " << inet_ntoa(newAddress.sin_addr) << std::endl;
			std::cout << "fd: " << newSocket << std::endl;
			std::cout << "---------------------------" << std::endl << std::endl;

			/* message    =  [ ":" prefix SPACE ] command [ params ] crlf
    		prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
    		command    =  1*letter / 3digit */
		}
		// Check if we received something from users
		else {
			for (i = 0; i < maxConnect; i++) {
				if (polls[i].revents == POLLIN) {
					std::cout << "fd: " << polls[i].fd << '\n';
					size = recv(polls[i].fd, &buffer, BUFFER_SIZE, 0);
					
					if (status == 0) {
						prefix.clear();
						prefix = ":wdebotte!Wiwi@";
						prefix += inet_ntoa(newAddress.sin_addr);
						command = prefix;
						command += " PASS password\n";
						send(newSocket, command.c_str(), command.size(), 0);
						prefix.clear();
						prefix = ":wdebotte!Wiwi@";
						prefix += inet_ntoa(newAddress.sin_addr);
						command = prefix;
						command += " NICK wdebotte\n";
						send(newSocket, command.c_str(), command.size(), 0);
						prefix.clear();
						prefix = ":wdebotte!Wiwi@";
						prefix += inet_ntoa(newAddress.sin_addr);
						command = prefix;
						command += " USER Wiwi Wiwi 127.0.0.1 :William\n";
						send(newSocket, command.c_str(), command.size(), 0);
						status = 1;
					}
					
					if (size == 0) {
						std::cout << "Fd " << polls[i].fd << " disconnected" << std::endl;
						close(polls[i].fd);
						polls[i].revents = POLLOUT;
						continue ;
					}
					buffer[size] = '\0';
					std::cout << buffer << std::endl;
				}
			}
		}
	}
	return 0;
}
