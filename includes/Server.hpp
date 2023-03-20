#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <string>
# include <iostream>
# include <sstream>
# include <vector>
# include <map>
# include <ctime>
# include <poll.h>
# include <sys/socket.h>
# include "Client.hpp"
# include "Config.hpp"
# include "Command.hpp"
# include "Channel.hpp"
# include "utils.hpp"

# define DEBUG 1

# define KRESET		"\x1B[0m"
# define KBOLD		"\x1b[1m"
# define KGRAY		"\x1B[90m"
# define KBLK		"\x1B[30m"
# define KRED		"\x1B[31m"
# define KGRN		"\x1B[32m"
# define KYEL		"\x1B[33m"
# define KBLU		"\x1B[34m"
# define KMAG		"\x1B[35m"
# define KCYN		"\x1B[36m"
# define KWHT		"\x1B[37m"
# define BROADCAST	"BROADCAST: "

# define TIMEOUT_LISTENING	500

class	Server {
	public:
		Server(const int &port, const std::string &password);
		~Server();

		int		initError(const int &exit_code, const std::string &error);
		int		init();

		void	acceptClient();
		void	run();
		void	receiveEntries(std::vector<pollfd>::iterator& it);
		void	deleteClients();

		void	sendPings();

		void	addChannel(Channel* channel);

		Channel*				getChannel(std::string name);
		int						getSocketFd() const;
		int						getPort() const;
		std::string&			getPassword();
		Config&					getConfig();
		Client*					getClient(std::string nickname);
		std::map<int, Client*>&	getClients();

	private:
		int									_fd;
		int									_port;
		std::string							_password;
		std::vector<pollfd>					_pfds;
		std::map<int, Client*>				_clients;
		std::map<std::string, Channel*>		_channels;
		Config								_config;
};

void	reply(unsigned short code, Client &client, std::string arg1 = "", std::string arg2 = "", std::string arg3 = "", std::string arg4 = "");

#endif
