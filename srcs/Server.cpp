#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Server.hpp"

Server::Server(const int &port, const std::string &password) : _fd(-1),
																_port(port),
																_password(password) {}

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

	#ifdef __linux__
		if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
			return initError(2, "can't set option(s) to server socket.");
	#endif

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
	char		buffer[4096];
	int			bytes;
	static int	lastPing = std::time(0);

	std::vector<pollfd>::iterator		it;
	std::map<int, Client*>::iterator	deleteIt;

	if (std::time(0) - lastPing >= atoi(this->_config.get("ping_delay").c_str())) {
		sendPings();
		lastPing = std::time(0);
	}
	if (poll(&this->_pfds[0], this->_pfds.size(), TIMEOUT_LISTENING) == -1)
		return ;

	if (this->_pfds[0].revents == POLLIN) // Server receive a new client connection
		this->acceptClient();
	else { // Check if client send something
		for (it = this->_pfds.begin(); it != this->_pfds.end(); it++) {
			if ((*it).revents == POLLIN) {
				bytes = recv((*it).fd, buffer, 4096, 0);
				buffer[bytes] = '\0';
				if (bytes == 0) {
					std::cout << KRED << BROADCAST << "Client " << KWHT << this->_clients[(*it).fd]->getNickname() << "(" << (*it).fd << ")" << KRED << " has been disconnected." << KRESET << std::endl;
					this->_clients[(*it).fd]->status = DISCONNECTED;
					// delete this->_clients[(*it).fd]; //TODO
					// this->_clients.erase((*it).fd);
					it = this->_pfds.erase(it);
					if (it == this->_pfds.end())
						break ;

					continue ;
				}
				if (this->_clients[(*it).fd]->status == COMMING // TODO : check for maximum users
					&& !this->_clients[(*it).fd]->getBaseInfos(this, buffer))
					continue ;
				else if (this->_clients[(*it).fd]->status == REGISTER) {
					std::cout << KGRN << BROADCAST << "Client " << KWHT << this->_clients[(*it).fd]->getNickname() << "(" << (*it).fd << ")" << KGRN << " has been connected." << KRESET << std::endl;
					this->_clients[(*it).fd]->connectToClient();
					this->_clients[(*it).fd]->status = CONNECTED;
					continue ;
				}
				manageEntry(buffer);
			}
		}
		// for (deleteIt = this->_clients.begin(); deleteIt != this->_clients.end(); deleteIt++) { // TODO : Check why looping
		// 	if ((*deleteIt).second->status == DISCONNECTED) {
		// 		delete (*deleteIt).second;
		// 		this->_clients.erase(deleteIt);
		// 		deleteIt = this->_clients.begin();	
		// 	}
		// }
	}
}

void	Server::sendPings() {
	std::map<int, Client*>::iterator	it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (std::time(NULL) - (*it).second->getLastPing() >= atoi(this->_config.get("timeout").c_str())) {
			(*it).second->status = DISCONNECTED;
		}
		else {
			(*it).second->send("PING " + (*it).second->getNickname());
			(*it).second->setLastPing(std::time(NULL)); // TODO - Manage in command PONG
		}
	}
}

void	Server::manageEntry(std::string entry) {
	size_t						pos = 0;
	size_t						lastPos;
	std::vector<std::string>	argv;

	entry.erase(std::remove(entry.begin(), entry.end(), '\r'), entry.end()); // Remove all '\r' because we don't want them
	while (pos < entry.size()) {
		lastPos = entry.find(' ', pos);
		if (lastPos == std::string::npos)
			lastPos = entry.size() - 1;

		argv.push_back(entry.substr(pos, lastPos - pos));
		pos = lastPos + 1;
	}

	std::vector<std::string>::iterator it;

	std::cout << "argv: ";
	for (it = argv.begin(); it != argv.end(); it++)
		std::cout << *it << ' ';

	std::cout << std::endl;
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
