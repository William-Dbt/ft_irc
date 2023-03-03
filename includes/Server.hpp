#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <string>
# include <iostream>
# include <sstream>
# include <vector>
# include <map>
# include <poll.h>
# include <sys/socket.h>
# include "Client.hpp"
# include "Config.hpp"

# define TIMEOUT_LISTENING	500 // TODO - Check for ping time

class	Server {
	public:
		Server(const int &port, const std::string &password);
		~Server();

		int		initError(const int &exit_code, const std::string &error);
		int		init();
		void	acceptClient();
		void	run();

		int				getSocketFd() const;
		int				getPort() const;
		std::string&	getPassword();
		Config&			getConfig();

		bool	running;

	private:
		int						_fd;
		int						_port;
		std::string				_password;
		std::vector<pollfd>		_pfds;
		std::map<int, Client*>	_clients;
		Config					_config;
};

void	reply (unsigned short code, Client &client, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");


#endif
