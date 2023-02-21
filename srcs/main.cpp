#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TIMEOUT_LISTENING	500

int	main(int argc, char *argv[]) {
	// TODO
	// if (argc != 3) {
	if (argc != 2) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}

	int	serverFd,
		opt;

	struct sockaddr_in	address;

	serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverFd < 0) {
		std::cerr << "Socket creation error" << std::endl;
		return 1;
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(argv[1]));

	// Set socket to non-blocking mode
	opt = 1;
	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	// open socket
	if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0 ) {
		std::cerr << "Error: bind socket failed." << std::endl;
		return 2;
	}


	int	i,
		maxConnect,
		nbDone,
		resu,
		newSocket;

	unsigned int		cliLenght;
	struct pollfd		*polls = NULL;
	struct sockaddr_in	newAddress;

	// Initialization of the pool of address that we will listen for
	maxConnect = 5;
	polls = new pollfd[maxConnect];
	for (i = 0; i < maxConnect; i++) {
		polls[i].fd = -1;
		polls[i].events = POLLIN;
		polls[i].revents = 0;
	}

	// Initialize the server
	polls[0].fd = serverFd;

	// Put the server on listening mode
	if (listen(serverFd, maxConnect) < 0) {
		std::cerr << "Error: listen failed." << std::endl;
		return 3;
	}

	int	n,
		found,
		bytes;

	char	buffer[4096];

	while (1) {
		nbDone = 0;
		resu = poll(polls, maxConnect, TIMEOUT_LISTENING);
		// Check if there is something to read
		if (resu < 0) {
			sleep(5);
			continue ;
		}
		// Yes ? Print what we received
		else if (resu > 0) {
			nbDone++;
			cliLenght = sizeof(newAddress);
			newSocket = accept(serverFd, (struct sockaddr*)&newAddress, &cliLenght);

			n = 0;
			found = 0;
			while (found == 0 && n < maxConnect) {
				if (polls[n].fd == -1) {
					found = 1;
					polls[n].fd = newSocket;
					std::cout << "----- New client -----" << std::endl;
					bytes = read(polls[n].fd, buffer, 4096);
					buffer[bytes - 2] = '\0';
					std::cout << buffer << std::endl;
					std::cout << "---------------------------" << std::endl << std::endl;
				}
				else
					n++;
			}

			// Something for the clients?
			// TODO blocking here
			i = 1;
			std::cout << nbDone << " " << resu << std::endl;
			while (nbDone <= resu && i < maxConnect) {
				std::cout << i << std::endl;
				// If there is a client, check if he send something
				if (polls[i].fd != -1 && polls[i].revents & POLLIN) {
					std::cout << "fds: " << polls[i].fd << std::endl;
					nbDone++;
					bytes = read(polls[i].fd, buffer, 4096);
					buffer[bytes - 2] = '\0';
					std::cout << "- " << buffer << std::endl;
				}
				i++;
			}
		}
	}
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
