#include <iostream>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"

Server::Server(const int &port, const std::string &password) : fd(-1), port(port), password(password) {}

Server::~Server() {
	if (this->fd != -1)
		close(this->fd);
}

int	Server::initError(const int &exit_code, const std::string &error) {
	std::cerr << "Error: " << error << std::endl;
	return exit_code;
}

int	Server::init() {
	int	socketFd = -1,
		opt = 1;

	struct sockaddr_in	address;

	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
		return initError(1, "can't open new socket.");

	if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		return initError(2, "can't set option(s) to server socket.");

	if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1)
		return (initError(3, "can't set the socket to O_NONBLOCK."));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->port);
	if (bind(socketFd, (struct sockaddr*)&address, sizeof(address)) == -1)
		return (initError(4, "can't assign address for the socket (using bind function)."));

	if (listen(socketFd, address.sin_port) == -1)
		return (initError(5, "can't listen to the given port."));

	this->fd = socketFd;
	this->pfds.push_back(pollfd());
	this->pfds.back().fd = socketFd;
	this->pfds.back().events = POLLIN;

	std::cout << "ServerFd: " << this->fd << '\n';
	std::cout << "Password: " << this->password << '\n';
	std::cout << "Port: " << this->port << std::endl;
	return 0;
}

void	Server::acceptClient() {
	int					fd;
	socklen_t			cliLenght;
	struct sockaddr_in	address;

	cliLenght = sizeof(address);
	fd = accept(this->fd, (struct sockaddr*)&address, &cliLenght);
	if (fd == -1)
		return ;

	this->pfds.push_back(pollfd());
	this->pfds.back().fd = fd;
	this->pfds.back().revents = POLLIN;
	std::cout << "----- New client -----\n";
	std::cout << "IP: " << inet_ntoa(address.sin_addr) << '\n';
	std::cout << "fd: " << fd << '\n';
	std::cout << "---------------------------\n" << std::endl;
}

void	Server::run() {
	std::vector<pollfd>::iterator	it;

	if (poll(&this->pfds[0], this->pfds.size(), TIMEOUT_LISTENING) == -1)
		return ;

	if (this->pfds[0].revents == POLLIN) // Server receive a new client connection
		this->acceptClient();
	else { // Check if client send something
		for (it = this->pfds.begin(); it != this->pfds.end(); it++) {
			if ((*it).revents == POLLIN) {
				std::cout << "fd: " << (*it).fd << '\n';
			}
		}
	}
}

int	Server::getSocketFd() const {
	return this->fd;
}

int	Server::getPort() const {
	return this->port;
}
