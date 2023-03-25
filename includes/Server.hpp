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

		void	addChannel(std::string name);
		void	deleteChannel(std::string name);
		void	kickClientFromChannel(Client* client, Channel* channel);
		void	kickClientFromAllChannels(Client* client);

		int						getSocketFd() const;
		int						getPort() const;
		std::string&			getPassword();
		Config&					getConfig();
		Client*					getClient(std::string nickname);
		std::map<int, Client*>&	getClients();
		Channel*				getChannel(std::string name);
		std::vector<Channel >	getChannels();

		bool					isClientInServer(std::string client);

	private:
		int									_fd;
		int									_port;
		std::string							_password;
		std::vector<pollfd>					_pfds;
		int									_nbClients;
		std::map<int, Client*>				_clients;
		std::map<std::string, Channel>		_channels;
		Config								_config;
};

#endif
