#include "../includes/Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

void Server::_init(int port)
{
	int master_socket_fd;

	/* @brief: Create a socket file descriptor
	** @prototype: int socket(int domain, int type, int protocol);
	**
	** @param {int}	domain: AF_INET for IPv4
	** @param {int}	type: SOCK_STREAM for TCP
	** @param {int}	protocol: 0 for default
	**
	** @return {int}: socket file descriptor, -1 for error
	*/
	master_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (master_socket_fd == -1)
	{
		std::cerr << "Error at socket(): " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	/* @brief: set options on sockets (SO_REUSEADDR, SO_REUSEPORT) to reuse address and port
	** @prototype: int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	**
	** @param {int}				socket: socket file descriptor
	** @param {int}				level: SOL_SOCKET for socket level options (SO_REUSEADDR, SO_REUSEPORT)
	** @param {int}				optname: SO_REUSEADDR for reuse address
	** @param {const void *}	optval: &opt for opt value (1 for true)
	** @param {int}				optlen: sizeof(opt) for opt value length
	**
	** @return {int}: 0 for success and -1 for error
	*/
	int opt = 1;
	if (setsockopt(master_socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		std::cerr << "Error at setsockopt(): " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	if (fcntl(master_socket_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Fcntl error" << std::endl;
		exit(EXIT_FAILURE);
	}

	//	struct sockaddr_in {
	//		short int			sin_family;		// Address family, AF_INET
	//		unsigned short int	sin_port;		// Port number
	//		struct in_addr		sin_addr;		// Internet address (IPV4)
	//		unsigned char		sin_zero[8];	// Same size as struct sockaddr
	//	};
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	/* @brief: bind a socket to an address and port number (IPV4)
	** @prototype: int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	**
	** @param {int}						socket: socket file descriptor
	** @param {const struct sockaddr *}	addr: struct sockaddr_in for IPV4
	** @param {socklen_t}				addrlen: sizeof(struct sockaddr_in)
	**
	** @return {int}: 0 for success and -1 for error
	*/
	if (bind(master_socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "Error at bind(): " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	/* @brief: listen for connections on a socket
	** @prototype: int listen(int sockfd, int backlog);
	**
	** @param {int}	socket: socket file descriptor
	** @param {int}	backlog: maximum length to which the queue of pending connections for sockfd may grow
	**
	** @return {int}: 0 for success and -1 for error
	*/
	if (listen(master_socket_fd, address.sin_port) < 0)
	{
		std::cerr << "Error at listen(): " << std::strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}

	int i;
	int maxConnect;

	struct pollfd *polls = NULL;

	maxConnect = 1000;

	// Initialization of the pool of address that we will listen for
	polls = new pollfd[maxConnect];
	for (i = 0; i < maxConnect; i++)
	{
		polls[i].fd = -1;
		polls[i].events = POLLIN;
	}

	// Initialize the server
	polls[0].fd = master_socket_fd;

	int newSocket; /* ,
		 bytes,
		 n,
		 found,
		 nbDone,
		 resu; */

	ssize_t size;
	char buffer[BUFFER_SIZE + 1];
	socklen_t cliLenght;
	struct sockaddr_in newAddress;
	std::string prefix;
	std::string command;
	// int status = 0;

	while (1)
	{
		if (poll(&polls[0], maxConnect, TIMEOUT_LISTENING) < 0)
			continue;

		// Is server receive something?
		if (polls[0].revents == POLLIN)
		{
			cliLenght = sizeof(newAddress);
			newSocket = accept(master_socket_fd, (struct sockaddr *)&newAddress, &cliLenght);
			if (newSocket == -1)
				continue;

			i = 0;
			while (i < maxConnect && polls[i].fd != -1)
				i++;

			polls[i].fd = newSocket;
			polls[i].revents = POLLIN;
			std::cout << "----- New client -----" << std::endl;
			std::cout << "IP: " << inet_ntoa(newAddress.sin_addr) << std::endl;
			std::cout << "fd: " << newSocket << std::endl;
			std::cout << "---------------------------" << std::endl
					  << std::endl;

			/* message    =  [ ":" prefix SPACE ] command [ params ] crlf
			prefix     =  servername / ( nickname [ [ "!" user ] "@" host ] )
			command    =  1*letter / 3digit */
		}
		// Check if we received something from users
		else
		{
			for (i = 0; i < maxConnect; i++)
			{
				if (polls[i].revents == POLLIN)
				{
					std::cout << "fd: " << polls[i].fd << '\n';
					size = recv(polls[i].fd, &buffer, BUFFER_SIZE, 0);

					// if (status == 0)
					// {
					// 	prefix.clear();
					// 	prefix = ":wdebotte!Wiwi@";
					// 	prefix += inet_ntoa(newAddress.sin_addr);
					// 	command = prefix;
					// 	command += " 001 PASS password\n";
					// 	send(newSocket, command.c_str(), command.size(), 0);
					// 	prefix.clear();
					// 	prefix = ":wdebotte!Wiwi@";
					// 	prefix += inet_ntoa(newAddress.sin_addr);
					// 	command = prefix;
					// 	command += " 002 NICK wdebotte\n";
					// 	send(newSocket, command.c_str(), command.size(), 0);
					// 	prefix.clear();
					// 	prefix = ":wdebotte!Wiwi@";
					// 	prefix += inet_ntoa(newAddress.sin_addr);
					// 	command = prefix;
					// 	command += " 003 USER Wiwi Wiwi 127.0.0.1 :William\n";
					// 	send(newSocket, command.c_str(), command.size(), 0);
					// 	status = 1;
					// }

					if (size == 0)
					{
						std::cout << "Fd " << polls[i].fd << " disconnected" << std::endl;
						close(polls[i].fd);
						polls[i].revents = POLLOUT;
						continue;
					}
					buffer[size] = '\0';
					std::cout << buffer << std::endl;
				}
			}
		}
	}

	void Server::_acceptUser(int server_fd)
	{

	}

	/* @brief: accept a connection on a socket
	** @prototype: int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	**
	** @param {int}					socket: socket file descriptor
	** @param {struct sockaddr *}	addr: struct sockaddr_in for IPV4
	** @param {socklen_t *}			addrlen: sizeof(struct sockaddr_in)
	** @return {int}: 0 for success and -1 for error
	*/
	// int new_socket;
	// int addrlen = sizeof(address);
	// if ((new_socket = accept(master_socket_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	// {
	// 	std::cerr << "Error at accept(): " << std::strerror(errno) << std::endl;
	// 	exit(EXIT_FAILURE);
	// }
	// std::cout << "Connection accepted : " << new_socket << std::endl;
}

// int is_map_playable(char **map)
// {
// 	int fd;
// 	int i;
// 	char *line;

// 	fd = open(map[1], O_RDONLY);
// 	while (1)
// 	{
// 		line = get_next_line(fd);
// 		if (!line)
// 			break;
// 		i = 0;
// 		while (line[i])
// 		{
// 			if (line[i] == '1' || line[i] == '0' || line[i] == 'N'
// 				|| line[i] == 'S' || line[i] == 'E' || line[i] == 'W')
// 			{
// 				free(line);
// 				return (1);
// 			}
// 			i++;
// 		}
// 	}
// 	return (0);
// }