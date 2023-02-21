#include "Server.hpp"

Server::Server()
{
}

Server::~Server()
{
}

void Server::_init()
{
	int socket_fd;

	/* @brief: Create a socket
	** @template: socket(domain, type, protocol)
	** @param {int} domain: AF_INET for IPv4
	** @param {int} type: SOCK_STREAM for TCP
	** @param {int} protocol: 0 for default
	** @return {int}: socket file descriptor, -1 for error
	*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd < 0)
	{
		std::cerr << "socket failed" << std::endl;
		return;
	}

	/* @brief: get and set options on sockets
	** @template: setsockopt(socket, level, optname, optval, optlen)
	** @param {int} socket: socket file descriptor
	** @param {int} level: SOL_SOCKET for socket level
	** @param {int} optname: SO_REUSEADDR for reuse address
	** @param {int} optval: &opt for opt
	** @param {int} optlen: sizeof(opt) for opt
	** @return {int}: 0 for success
	*/
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
}