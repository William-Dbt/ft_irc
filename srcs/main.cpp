#include <iostream>
// #include <cstdlib>
// #include <unistd.h>
// #include <fcntl.h>
// #include <poll.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>

#include "Server.hpp"

// #define TIMEOUT_LISTENING	500
// #define BUFFER_SIZE			4096

int	main(int argc, char *argv[]) {
	Server	server;

	// TODO
	// if (argc != 3) {
	if (argc != 2) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	if (server.init(argv[1]))
		return 1;

	while (server.getStatus() == ON)
		server.run();

	// int	serverFd,
	// 	opt;

	// serverFd = socket(AF_INET, SOCK_STREAM, 0);
	// if (serverFd < 0) {
	// 	std::cerr << "Socket creation error" << std::endl;
	// 	return 1;
	// }
	// // Set socket to non-blocking mode
	// if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
	// 	std::cerr << "Set socket option error" << std::endl;
	// 	return 293235;
	// }
	// if (fcntl(serverFd, F_SETFL, O_NONBLOCK) < 0) {
	// 	std::cerr << "Fcntl error" << std::endl;
	// 	return 145;
	// }

	// struct sockaddr_in	address;

	// address.sin_family = AF_INET;
	// address.sin_addr.s_addr = INADDR_ANY;
	// address.sin_port = htons(atoi(argv[1]));

	// // open socket
	// if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
	// 	std::cerr << "Error: bind socket failed." << std::endl;
	// 	return 2;
	// }

	// if (listen(serverFd, address.sin_port) < 0) {
	// 	std::cerr << "Error: listen failed." << std::endl;
	// 	return 3;
	// }


	// int	i,
	// 	maxConnect;

	// struct pollfd		*polls = NULL;

	// maxConnect = 1000;

	// // Initialization of the pool of address that we will listen for
	// polls = new pollfd[maxConnect];
	// for (i = 0; i < maxConnect; i++) {
	// 	polls[i].fd = -1;
	// 	polls[i].events = POLLIN;
	// }

	// // Initialize the server
	// polls[0].fd = serverFd;

	// int	newSocket,
	// 	bytes;/*
	// 	n,
	// 	found,
	// 	nbDone,
	// 	resu; */

	// char				buffer[4096];
	// socklen_t			cliLenght;
	// struct sockaddr_in	newAddress;

	// while (1) {
	// 	if (poll(&polls[0], maxConnect, TIMEOUT_LISTENING) < 0)
	// 		continue ;

	// 	if (polls[0].revents == POLLIN) {
	// 		cliLenght = sizeof(newAddress);
	// 		newSocket = accept(serverFd, (struct sockaddr*)&newAddress, &cliLenght);
	// 		if (newSocket == -1)
	// 			continue ;

	// 		i = 0;
	// 		while (polls[i].fd != -1)
	// 			i++;

	// 		polls[i].fd = newSocket;
	// 		std::cout << "----- New client -----" << std::endl;
	// 		std::cout << "IP: " << inet_ntoa(newAddress.sin_addr) << std::endl;
	// 		std::cout << "fd: " << newSocket << std::endl;
	// 		std::cout << "---------------------------" << std::endl << std::endl;

	// 		bytes = recv(newSocket, buffer, sizeof(buffer), MSG_DONTWAIT);
	// 		buffer[bytes] = '\0';
	// 		std::cout << buffer << std::endl << std::endl;
	// 		// bytes = send(newSocket, ": NICK :wdebotte", 16, MSG_DONTWAIT);
	// 	}
	// 	else {
	// 		for (i = 0; i < maxConnect; i++) {
	// 			if (polls[i].fd != -1 && polls[i].revents == POLLIN) {
	// 				std::cout << polls[i].fd << " received something" << std::endl;
	// 				ssize_t	size;

	// 				size = recv(polls[i].fd, &buffer, BUFFER_SIZE, 0);
	// 				if (size == 0) {
	// 					std::cout << "Disconnected" << std::endl;
	// 					// close(polls[i].fd);
	// 					// polls[i].fd = -1;
	// 					continue ;
	// 				}

	// 				buffer[size] = '\0';

	// 				// std::cout << buffer << std::endl;
	// 			}
	// 		}
	// 	}

	// 	/* nbDone = 0;
	// 	resu = poll(&polls[0], maxConnect, TIMEOUT_LISTENING);
	// 	// Check if there is something to read
	// 	if (resu < 0) {
	// 		sleep(5);
	// 		continue ;
	// 	}
	// 	// Yes ? Print what we received
	// 	else if (resu > 0) {
	// 		nbDone++;
	// 		cliLenght = sizeof(newAddress);
	// 		newSocket = accept(serverFd, (struct sockaddr*)&newAddress, &cliLenght);
	// 		if (newSocket < 0)
	// 			continue ;

	// 		n = 0;
	// 		found = 0;
	// 		while (found == 0 && n < maxConnect) {
	// 			if (polls[n].fd == -1) {
	// 				found = 1;
	// 				polls[n].fd = newSocket;
	// 				std::cout << "----- New client -----" << std::endl;
	// 				bytes = recv(polls[n].fd, buffer, 4096, MSG_PEEK);
	// 				buffer[bytes - 2] = '\0';
	// 				std::cout << buffer << std::endl;
	// 				std::cout << "---------------------------" << std::endl << std::endl;
	// 			}
	// 			else
	// 				n++;
	// 		}

	// 		// Something for the clients?
	// 		i = 1;
	// 		while (nbDone <= resu && i < maxConnect) {
	// 			// If there is a client, check if he send something
	// 			if (polls[i].fd != -1 && polls[i].revents & POLLIN) {
	// 				nbDone++;
	// 				bytes = recv(polls[i].fd, buffer, 4096, MSG_PEEK);
	// 				buffer[bytes - 2] = '\0';
	// 				std::cout << "- " << buffer << std::endl;
	// 			}
	// 			i++;
	// 		}
	// 	} */
	// }
	return 0;
}

// int main()
// {
//     int server_fd, new_socket, valread;
//     struct sockaddr_in address;
//     int opt = 1;
//     int addrlen = sizeof(address);
//     char buffer[1024] = { 0 };
//     std::string hello = "Hello from server";
  
//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }
  
//     // Forcefully attaching socket to the port 8080
//     if (setsockopt(server_fd, SOL_SOCKET,
//                    SO_REUSEADDR | SO_REUSEPORT, &opt,
//                    sizeof(opt))) {
//         perror("setsockopt");
//         exit(EXIT_FAILURE);
//     }
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);
  
//     // Forcefully attaching socket to the port 8080
//     if (bind(server_fd, (struct sockaddr*)&address,
//              sizeof(address))
//         < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }
//     if (listen(server_fd, 3) < 0) {
//         perror("listen");
//         exit(EXIT_FAILURE);
//     }
//     if ((new_socket
//          = accept(server_fd, (struct sockaddr*)&address,
//                   (socklen_t*)&addrlen))
//         < 0) {
//         perror("accept");
//         exit(EXIT_FAILURE);
//     }
//     valread = read(new_socket, buffer, 1024);
//     printf("%i, %s\n", valread, buffer);
//     send(new_socket, hello.c_str(), strlen(hello.c_str()), 0);
//     printf("Hello message sent\n");
  
//     // closing the connected socket
//     close(new_socket);
//     // closing the listening socket
//     shutdown(server_fd, SHUT_RDWR);
//     return 0;
// }
