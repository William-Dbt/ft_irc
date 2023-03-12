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

	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		initError(1, "can't set a client's fd to O_NONBLOCK.");

	this->_pfds.push_back(pollfd());
	this->_pfds.back().fd = fd;
	this->_pfds.back().events = POLLIN;
	this->_clients[fd] = new Client(fd, inet_ntoa(address.sin_addr), this);
}

void	Server::run() {
	static int	lastPing = std::time(NULL);

	std::vector<pollfd>::iterator	it;

	if (poll(&this->_pfds[0], this->_pfds.size(), atoi(this->_config.get("timeout").c_str())) == -1)
		return ;

	if (std::time(NULL) - lastPing >= atoi(this->_config.get("ping_delay").c_str())) {
		sendPings();
		lastPing = std::time(NULL);
	}
	else {
		if (this->_pfds[0].revents == POLLIN) // Server receive a new client connection
			this->acceptClient();
		else { // Check if client send something
			for (it = this->_pfds.begin(); it != this->_pfds.end(); it++) {
				if ((*it).revents == POLLIN)
					receiveEntries(it);
			}
		}
	}
	deleteClients();
}

void	Server::receiveEntries(std::vector<pollfd>::iterator& it) {
	char		buffer[4096];
	int			bytes;

	bytes = recv((*it).fd, buffer, 4096, 0);
	buffer[bytes] = '\0';
	if (bytes == 0) {
		std::cout << KRED << BROADCAST << "Client " << KWHT << this->_clients[(*it).fd]->getNickname() << "(" << (*it).fd << ")" << KRED << " has been disconnected." << KRESET << std::endl;
		this->_clients[(*it).fd]->status = DISCONNECTED;
		return ;
	}
	if (this->_clients[(*it).fd]->status == COMMING
		&& !this->_clients[(*it).fd]->getBaseInfos(buffer)) {
		return ;
	}
	else if (this->_clients[(*it).fd]->status == REGISTER) {
		std::cout << KGRN << BROADCAST << "Client " << KWHT << this->_clients[(*it).fd]->getNickname() << "(" << (*it).fd << ")" << KGRN << " has been connected." << KRESET << std::endl;
		this->_clients[(*it).fd]->connectToClient();
		this->_clients[(*it).fd]->status = CONNECTED;
		return ;
	}
	
	Command	command(this->_clients[(*it).fd], buffer);

	command.execute();
}

static void	deleteClientPollFd(std::vector<pollfd>& pfds, int& fd) {
	std::vector<pollfd>::iterator	it;

	for (it = pfds.begin(); it != pfds.end(); it++) {
		if ((*it).fd == fd) {
			pfds.erase(it);
			break ;
		}
	}
}

void	Server::deleteClients() {
	std::map<int, Client*>::iterator	it;

	std::vector<Client*>				usersToDelete;
	std::vector<Client*>::iterator		deleteIt;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++)
		if ((*it).second->status == DISCONNECTED)
			usersToDelete.push_back((*it).second);

	if (!usersToDelete.size())
		return ;

	for (deleteIt = usersToDelete.begin(); deleteIt != usersToDelete.end(); deleteIt++) {
		deleteClientPollFd(this->_pfds, (*deleteIt)->getFd());
		this->_clients.erase((*deleteIt)->getFd());
		delete (*deleteIt);
	}
}

void	Server::sendPings() {
	int	timeout = atoi(this->_config.get("timeout").c_str());

	std::map<int, Client*>::iterator	it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if (std::time(NULL) - (*it).second->getLastPing() >= timeout)
			(*it).second->status = DISCONNECTED;
		else {
			(*it).second->send("PING " + (*it).second->getNickname());
			(*it).second->setLastPing(std::time(NULL));
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
