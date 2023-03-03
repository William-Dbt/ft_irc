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

Server::Server(const int &port, const std::string &password) : _fd(-1),
																_port(port),
																_password(password) {
	this->running = true;
}

Server::~Server() {
	std::map<int, Client*>::iterator	it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++)
		delete (*it).second;

	if (this->_fd != -1)
		close(this->_fd);
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
		/* return initError(2, "can't set option(s) to server socket.") */;

	if (fcntl(socketFd, F_SETFL, O_NONBLOCK) == -1)
		return (initError(3, "can't set the socket to O_NONBLOCK."));

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->_port);
	if (bind(socketFd, (struct sockaddr*)&address, sizeof(address)) == -1)
		return (initError(4, "can't assign address for the socket (using bind function)."));

	if (listen(socketFd, address.sin_port) == -1)
		return (initError(5, "can't listen to the given port."));

	this->_fd = socketFd;
	this->_pfds.push_back(pollfd());
	this->_pfds.back().fd = socketFd;
	this->_pfds.back().events = POLLIN;
	return 0;
}

void	Server::acceptClient() {
	int					fd;
	socklen_t			cliLenght;
	struct sockaddr_in	address;

	cliLenght = sizeof(address);
	fd = accept(this->_fd, (struct sockaddr*)&address, &cliLenght);
	if (fd == -1)
		return ;

	this->_pfds.push_back(pollfd());
	this->_pfds.back().fd = fd;
	this->_pfds.back().events = POLLIN;
	this->_clients[fd] = new Client(fd, inet_ntoa(address.sin_addr));
}

void	Server::run() {
	unsigned int					toDelete = 0;
	std::vector<pollfd>::iterator	it;
	std::vector<pollfd>::iterator	deleteIt;

	if (poll(&this->_pfds[0], this->_pfds.size(), TIMEOUT_LISTENING) == -1)
		return ;

	if (this->_pfds[0].revents == POLLIN) // Server receive a new client connection
		this->acceptClient();
	else { // Check if client send something
		for (it = this->_pfds.begin(); it != this->_pfds.end(); it++) {
			if ((*it).revents == POLLIN) {
				char	buffer[4096];

				int bytes = recv((*it).fd, buffer, 4096, 0);
				buffer[bytes] = '\0';
				if (bytes == 0) {
					std::cout << "fd " << (*it).fd << " disconnected." << std::endl;
					delete this->_clients[(*it).fd];
					this->_clients.erase((*it).fd);
					toDelete++;
					(*it).revents = POLLOUT;
					continue ;
				}
				if (this->_clients[(*it).fd]->status == COMMING
					&& !this->_clients[(*it).fd]->getBaseInfos(this, buffer))
					continue ;
				else if (this->_clients[(*it).fd]->status == REGISTER) {
					std::cout << "--- New client registered ---\n";
					std::cout << "fd: " << this->_clients[(*it).fd]->getFd() << '\n';
					std::cout << "host: " << this->_clients[(*it).fd]->getHost() << '\n';
					std::cout << "pass: " << this->_clients[(*it).fd]->getPassword() << '\n';
					std::cout << "nickname: " << this->_clients[(*it).fd]->getNickname() << '\n';
					std::cout << "username: " << this->_clients[(*it).fd]->getUsername() << '\n';
					std::cout << "realname: " << this->_clients[(*it).fd]->getRealname() << '\n';
					std::cout << "-----------------------------" << std::endl << std::endl;
					this->_clients[(*it).fd]->connectToClient();
					this->_clients[(*it).fd]->status = CONNECTED;
				}
				std::cout << buffer << '\n' << std::endl;
			}
		}
		while (toDelete) {
			for (deleteIt = this->_pfds.begin(); deleteIt != this->_pfds.end(); deleteIt++) {
				if ((*deleteIt).revents == POLLOUT) {
					this->_pfds.erase(deleteIt);
					toDelete--;
					break ;
				}
			}
		}
	}
}

int	Server::getSocketFd() const {
	return this->_fd;
}

int	Server::getPort() const {
	return this->_port;
}

std::string&	Server::getPassword() {
	return this->_password;
}

Config&	Server::getConfig() {
	return this->_config;
}
