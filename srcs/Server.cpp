#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"
#include "Client.hpp"

Server::Server() {}

int	Server::getStatus() const {
	return this->status;
}

int	Server::init(char* port) {
	int					opt = 1;
	struct sockaddr_in	address;

	this->fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->fd == -1) {
		std::cerr << "Error: socket creation failed." << std::endl;
		return 1;
	}
	if (setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
		std::cerr << "Error: set socket option failed." << std::endl;
		return 1;
	}
	if (fcntl(this->fd, F_SETFL, O_NONBLOCK) < 0) {
		std::cerr << "Error: fcntl failed." << std::endl;
		return 1;
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(port));
	if (bind(this->fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
		std::cerr << "Error: binding socket failed." << std::endl;
		return 1;
	}
	if (listen(this->fd, address.sin_port) == -1) {
		std::cerr << "Error: listen on fd " << this->fd << " failed." << std::endl;
		return 1;
	}
	this->polls.push_back(pollfd());
	this->polls.back().fd = this->fd;
	this->polls.back().revents = POLLIN;
	this->status = ON;
	return 0;
}

void	Server::run() {
	if (poll(&this->polls[0], 2000, TIMEOUT) < 0)
		return ;

	std::cout << "Titi" << std::endl;
	if (this->polls[0].revents == POLLIN) {
		acceptClient();
	}
	else {
		std::cout << "Check for new entry" << std::endl;
	}
}

void	Server::acceptClient() {
	int					fd;
	socklen_t			cliLenght;
	struct sockaddr_in	address;

	cliLenght = sizeof(address);
	std::cout << this->fd << std::endl;
	fd = accept(this->fd, (struct sockaddr*)&address, &cliLenght);
	if (fd == -1)
		return ;

	this->polls.push_back(pollfd());
	this->polls.back().fd = fd;
	this->polls.back().revents = POLLIN;
	// this->clients.push_back(Client(fd));
	std::cout << "----- New client -----" << std::endl;
	std::cout << "IP: " << inet_ntoa(address.sin_addr) << std::endl;
	std::cout << "fd: " << fd << std::endl;
	std::cout << "---------------------------" << std::endl << std::endl;
}
