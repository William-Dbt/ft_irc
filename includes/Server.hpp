#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <string>
# include <vector>
# include <map>
# include <poll.h>
# include "Client.hpp"

# define TIMEOUT_LISTENING	500 // TODO - Check for ping time

class	Server {
	public:
		Server(const int &port, const std::string &password);
		~Server();

		int		initError(const int &exit_code, const std::string &error);
		int		init();
		void	acceptClient();
		void	run();

		int			getSocketFd() const;
		int			getPort() const;
		std::string	getPassword() const;

		bool	running;

	private:
		int						fd;
		int						port;
		std::string				password;
		std::vector<pollfd>		pfds;
		std::map<int, Client*>	clients;
};

#endif
