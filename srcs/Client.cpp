#include "Client.hpp"

Client::Client() :	status(DISCONNECTED),
					fd(-1),
					host("0.0.0.0"),
					password("unknown"),
					nickname("unknown"),
					username("unknown"),
					realname("unknown") {}

Client::~Client() {
	if (this->fd != -1)
		close(this->fd);
}


