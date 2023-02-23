// #include <netinet/in.h>
// #include <iostream>
// #include <cstring>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <arpa/inet.h>
#include "Server.hpp"

// #define PORT 8080

// int main(int argc, char *argv[])
// {
// 	(void)argc;
// 	(void)argv;

// 	Server server;

// 	server._init(PORT);



// 	// while (true)
// 	// {
// 	// 	int server_fd;
// 	// 	int new_socket;
// 	// 	int valread;
// 	// 	struct sockaddr_in address;
// 	// 	int opt = 1;
// 	// 	socklen_t addrlen = sizeof(address);
// 	// 	char buffer[1024] = {0};
// 	// 	const char *hello = "Hello from server";

// 	// 	// Creating socket file descriptor
// 	// 	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 	// 	{
// 	// 		std::cerr << "socket failed" << std::endl;
// 	// 		return (1);
// 	// 	}

// 	// 	// Forcefully attaching socket to the port 8080
// 	// 	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
// 	// 	{
// 	// 		std::cerr << "setsockopt failed" << std::endl;
// 	// 		return (1);
// 	// 	}
// 	// 	address.sin_family = AF_INET;
// 	// 	address.sin_addr.s_addr = INADDR_ANY;
// 	// 	address.sin_port = htons(PORT);

// 	// 	// Forcefully attaching socket to the port 8080
// 	// 	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
// 	// 	{
// 	// 		std::cerr << "bind failed" << std::endl;
// 	// 		return (1);
// 	// 	}
// 	// 	if (listen(server_fd, 3) < 0)
// 	// 	{
// 	// 		std::cerr << "listen failed" << std::endl;
// 	// 		return (1);
// 	// 	}
// 	// 	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
// 	// 	{
// 	// 		std::cerr << "accept failed" << std::endl;
// 	// 		return (1);
// 	// 	}
// 	// 	valread = read(new_socket, buffer, 1024);
// 	// 	std::cout << buffer << std::endl;
// 	// 	send(new_socket, hello, strlen(hello), 0);
// 	// 	std::cout << "Hello message sent" << std::endl;

// 	// 	// closing the connected socket
// 	// 	close(new_socket);
// 	// 	// closing the listening socket
// 	// 	shutdown(server_fd, SHUT_RDWR);
// 	// }
// 	return 0;
// }


int main(int argc, char *argv[])
{
	Server server;
	
	if (argc != 2)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	try
	{
		server._init(atoi(argv[1]));
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	
	return 0;
}

