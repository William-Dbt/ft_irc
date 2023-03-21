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

	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		(*it).second->sendTo("QUIT :Server disconnected");
		delete (*it).second;
	}
	if (this->_fd != -1)
		close(this->_fd);
}

int	Server::initError(const int &exit_code, const std::string &error) {
	std::cerr << "Error: " << error << std::endl;
	return exit_code;
}

int	Server::init() {
	int socketFd = -1;
	int opt = 1;

	struct sockaddr_in	address;

	socketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketFd == -1)
		return initError(1, "can't open new socket.");

	#ifdef __linux__
		if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
			return initError(2, "can't set option(s) to server socket.");
	#else
		if (setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
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
		if (this->_pfds[0].revents == POLLIN)
			this->acceptClient();
		else {
			for (it = this->_pfds.begin(); it != this->_pfds.end(); it++) {
				if ((*it).revents == POLLIN)
					receiveEntries(it);
			}
		}
	}
	deleteClients();
}

void	Server::receiveEntries(std::vector<pollfd>::iterator& it) {
	char		readBuffer[4096];
	int			bytes;
	Client*		user = this->_clients[(*it).fd];

	bytes = recv((*it).fd, readBuffer, 4096, 0);
	readBuffer[bytes] = '\0';

	if (DEBUG)
		std::cout << KGRAY << getCurrentDateTime(0,0) << KRESET
			<< KBOLD << "   <--" << KGRAY << "{"<< (*it).fd << "}" << KRESET
			<< KBOLD << "[" << KRESET
			<< KMAG << readBuffer << KRESET
			<< std::endl;

	if (bytes == 0) {
		user->status = DISCONNECTED;
		return ;
	}

	size_t		pos = 0;
	size_t		lastPos;
	std::string	entryBuffer = readBuffer;
	std::string	commandBuffer;

	while (pos != entryBuffer.size()) {
		lastPos = entryBuffer.find("\r\n", pos) + 2;
		commandBuffer = entryBuffer.substr(pos, lastPos - pos);
		pos = lastPos;
		if (commandBuffer.find("CAP LS") != std::string::npos) // Skip the first line (doesn't know what is it for)
			continue ;

		Command	command(this->_clients[(*it).fd], commandBuffer);
		command.execute();
	}
	if (user->status == FULLYREGISTER) {
		if (user->getNickname().empty()) {
			user->status = DISCONNECTED;
			return ;
		}
		std::cout << KGRN << BROADCAST << "Client " << KWHT << user->getNickname() << "(" << (*it).fd << ")" << KGRN << " has been connected." << KRESET << std::endl;
		user->status = CONNECTED;
		user->connectToClient(*this);
	}
}

static void	deleteClientPollFd(std::vector<pollfd>& pfds, int fd) {
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
		(*deleteIt)->sendTo("QUIT :" + (*deleteIt)->getQuitMessage());
		std::cout << KRED << BROADCAST << "Client " << KWHT << (*deleteIt)->getNickname() << "(" << (*deleteIt)->getFd() << ")" << KRED << " has been disconnected." << KRESET << std::endl;
		deleteClientPollFd(this->_pfds, (*deleteIt)->getFd());
		this->_clients.erase((*deleteIt)->getFd());
		delete (*deleteIt);
	}
}

void	Server::sendPings() {
	int	timeout = atoi(this->_config.get("timeout").c_str());

	std::map<int, Client*>::iterator	it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if ((*it).second->status != CONNECTED)
			continue ;

		if (std::time(NULL) - (*it).second->getLastPing() >= timeout)
			(*it).second->status = DISCONNECTED;
		else
			(*it).second->send("PING " + (*it).second->getNickname());
	}

	// TMP
	std::map<std::string, Channel*>::iterator	chanIt;
	std::map<int, Client*>::iterator		clientIt;

	std::cout << KGRN << "Channels:" << (this->_channels.begin() == this->_channels.end()) << KRESET << std::endl;
	for (chanIt = this->_channels.begin(); chanIt != this->_channels.end(); chanIt++) {
		std::cout << KGRN << "Channel " << KBOLD << (*chanIt).first << KRESET << std::endl;
		std::cout << KGRN << "key: " << KBOLD <<  (*chanIt).second->getKey() << KRESET << std::endl;
		std::cout << KGRN << "topic: " << KBOLD <<  (*chanIt).second->getTopic() << KRESET << std::endl;

		std::map<int, Client*>					clientInChannel = (*chanIt).second->getClients();
		std::map<int, Client*>::iterator		clientInChannelIt;

		for (clientInChannelIt = clientInChannel.begin(); clientInChannelIt != clientInChannel.end(); clientInChannelIt++) {
			std::cout << KGRN << "- " << (*clientInChannelIt).second->getNickname() << KRESET << std::endl;
		}
		std::cout << std::endl;
	}
	for (clientIt = this->_clients.begin(); clientIt != this->_clients.end(); clientIt++) 
		std::cout << KYEL << "Client " << (*clientIt).second->getNickname() << KRESET << std::endl;
}

void	Server::addChannel(Channel *channel)
{
	this->_channels[channel->getName()] = channel;
}

void	Server::deleteChannel(std::string name)
{
	std::map<std::string, Channel*>::iterator	it;

	it = this->_channels.find(name);
	if (it == this->_channels.end())
		return ;
	this->_channels.erase(it);
}

Channel*	Server::getChannel(std::string name) {
	std::map<std::string, Channel*>::iterator	it;

	it = this->_channels.find(name);
	if (it == this->_channels.end())
		return NULL;
	return (*it).second;
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

Client*	Server::getClient(std::string nickname) {
	std::map<int, Client*>::iterator	it;

	for (it = this->_clients.begin(); it != this->_clients.end(); it++)
		if ((*it).second->getNickname() == nickname)
			return (*it).second;

	return NULL;
}

std::map<int, Client*>&	Server::getClients() {
	return this->_clients;
}

std::vector<Channel *> Server::getChannels()
{
	std::map<std::string, Channel*>::iterator	chanIt;
	std::vector<Channel *>						channels;
	for (chanIt = this->_channels.begin(); chanIt != this->_channels.end(); chanIt++) {
		channels.push_back((*chanIt).second);
	}
	return channels;
}