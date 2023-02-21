#include <sys/socket.h> // for socket(), bind(), listen(), accept()
#include <sys/types.h>	// for socket types (SOCK_STREAM, SOCK_DGRAM)
#include <netinet/in.h> // for sockaddr_in and inet_addr()
#include <iostream>

class Server
{
private:
public:
	Server();
	~Server();
	void _init(int port);
};