#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include <string>
# include <vector>
# include <unistd.h>

class	Server;

enum	e_status {
	COMMING = 0,
	REGISTER,
	CONNECTED
};

class	Client {
	public:
		Client(const int& fd, const std::string& host);
		~Client();

		void	connectToClient();
		void	setBaseInfo(std::string& entryInfo, std::string& serverPassword);
		bool	getBaseInfos(Server* server, std::string entry);

		std::string	getPrefix();

		int&			getFd();
		std::string&	getHost();
		std::string&	getPassword();
		std::string&	getNickname();
		std::string&	getUsername();
		std::string&	getRealname();

		int	status;

	private:
		int							_fd;
		std::string					_host;
		std::string					_password;
		std::string					_nickname;
		std::string					_username;
		std::string					_realname;
		std::vector<std::string>	_commands;
};

# include "Server.hpp"

#endif
