#include "Client.hpp"

Client::Client(int const& fd) : fd(fd) {}

int	Client::getFd() const {
	return this->fd;
}
