#include "../includes/Server.hpp"

Server::Server()
{
	// _init(8080);
}

Server::~Server()
{
}

void Server::_init(int port)
{
	int socket_fd;
	int opt;
	struct sockaddr_in address;

	/* @brief: Create a socket file descriptor
	** @prototype: int socket(int domain, int type, int protocol);
	**
	** @param {int}	domain: AF_INET for IPv4
	** @param {int}	type: SOCK_STREAM for TCP
	** @param {int}	protocol: 0 for default
	**
	** @return {int}: socket file descriptor, -1 for error
	*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		std::cerr << "socket failed" << std::endl;
		return;
	}
	std::cout << "socket_fd: " << socket_fd << std::endl;

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
	opt = 1;
	if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
	{
		std::cerr << "setsockopt failed" << std::endl;
		return;
	}
	std::cout << "setsockopt success" << std::endl;

	/* @brief: bind a socket to an address and port number (IPV4)
	** @prototype: int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	**
	** @param {int}						socket: socket file descriptor
	** @param {const struct sockaddr *}	addr: struct sockaddr_in for IPV4
	** @param {socklen_t}				addrlen: sizeof(struct sockaddr_in)
	**
	** @return {int}: 0 for success and -1 for error
	*/

	//	struct sockaddr_in {
	//		short int			sin_family;		// Address family, AF_INET
	//		unsigned short int	sin_port;		// Port number
	//		struct in_addr		sin_addr;		// Internet address (IPV4)
	//		unsigned char		sin_zero[8];	// Same size as struct sockaddr
	//	};
	address.sin_family = AF_INET; // IPV4 address family (AF_INET6 for IPV6)
	address.sin_addr.s_addr = INADDR_ANY; // any incoming interface
	address.sin_port = htons(port); // port number to bind to (htons for host to network short) 
	if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		std::cerr << "bind failed" << std::endl;
		return;
	}
	std::cout << "bind success" << std::endl;

	/* @brief: listen for connections on a socket
	** @prototype: int listen(int sockfd, int backlog);
	**
	** @param {int}	socket: socket file descriptor
	** @param {int}	backlog: maximum length to which the queue of pending connections for sockfd may grow
	**
	** @return {int}: 0 for success and -1 for error
	*/
	if (listen(socket_fd, 3) < 0)
	{
		std::cerr << "listen failed" << std::endl;
		return;
	}
	std::cout << "listen success" << std::endl;

	/* @brief: accept a connection on a socket
	** @prototype: int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	**
	** @param {int}					socket: socket file descriptor
	** @param {struct sockaddr *}	addr: struct sockaddr_in for IPV4
	** @param {socklen_t *}			addrlen: sizeof(struct sockaddr_in)
	**
	** @return {int}: 0 for success and -1 for error
	*/
	int new_socket;
	int addrlen = sizeof(address);
	if ((new_socket = accept(socket_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
	{
		std::cerr << "accept failed" << std::endl;
		return;
	}
	std::cout << "accept success" << std::endl;
}