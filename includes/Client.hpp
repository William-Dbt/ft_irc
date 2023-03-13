#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include <string>
# include <vector>
# include <map>
# include <ctime>
# include <unistd.h>
# include "Command.hpp"

class	Server;

enum	e_status {
	COMMING = 0,
	REGISTER,
	CONNECTED,
	DISCONNECTED
};

class	Client {
	public:
		Client(const int& fd, const std::string& host, Server* server);
		~Client();

		void	connectToClient();
		void	setBaseInfo(std::string& entryInfo, std::string& serverPassword);
		bool	getBaseInfos(std::string entry);

		void	send(std::string message);

		std::string	getPrefix();

		void	setLastPing(time_t time);

		time_t&			getLastPing();
		int&			getFd();
		std::string&	getHost();
		std::string&	getPassword();
		std::string&	getNickname();
		std::string&	getUsername();
		std::string&	getRealname();

		std::map<std::string, void (*)(Command*)>&	getCommands();

		int	status;

	private:
		time_t		_lastPing;
		int			_fd;
		std::string	_host;
		std::string	_password;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;

		Server*										_server;
		std::map<std::string, void (*)(Command*)>	_commands;
};

# include "Server.hpp"

#endif
