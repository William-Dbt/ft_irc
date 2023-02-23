#include <sys/socket.h> // for socket(), bind(), listen(), accept()
#include <sys/types.h>	// for socket types (SOCK_STREAM, SOCK_DGRAM)
#include <netinet/in.h> // for sockaddr_in and inet_addr()
#include <iostream> 
#include <cerrno> // for errno
#include <cstring> // for strerror()
#include <cstdlib> // for exit()
#include <fcntl.h> // for fcntl()
#include <poll.h> // for poll()
#include <unistd.h> // for close()
#include <arpa/inet.h> // for inet_ntoa()

#define TIMEOUT_LISTENING	500
#define BUFFER_SIZE			4096

class Server
{
private:
public:
	Server();
	~Server();
	void _init(int port);
	void _acceptUser(int server_fd);
};