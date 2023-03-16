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
	BADPASSWORD = 0,
	COMMING,
	REGISTER,
	CONNECTED,
	DISCONNECTED
};

class	Channel;

class	Client {
	public:
		Client(const int& fd, const std::string& host, Server* server);
		~Client();

		void	connectToClient(Server &server);
		bool	getBaseInfos(std::string entry);

		void	send(std::string message);
		void	sendReply(std::string message);

		std::string getPrefix();

		void	addChannel(Channel* channel);

		void	setLastPing(time_t time);
		void	setPassword(std::string password);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
		void	setRealname(std::string realname);

		time_t&			getLastPing();
		int&			getFd();
		std::string&	getHost();
		std::string&	getPassword();
		std::string&	getNickname();
		std::string&	getUsername();
		std::string&	getRealname();
		Server*			getServer();

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
		std::map<std::string, Channel*>				_channels;

		std::map<std::string, void (*)(Command*)>	_commands;
};

# include "Server.hpp"
# include "Channel.hpp"

#endif
