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
	FULLYREGISTER,
	CONNECTED,
	DISCONNECTED
};

class	Channel;

class	Client {
	public:
		Client(const int& fd, const std::string& host, Server* server);
		~Client();

		void	connectToClient(Server &server);

		void	send(std::string message);
		void	sendTo(std::string message);
		void	sendReply(std::string message);

		std::string	getPrefix();
		bool		isModeInUse(char mode);
		void		addMode(char mode);
		void		removeMode(char mode);

		void	addChannel(Channel* channel);
		void	leaveChannel(Channel* channel);
		void	leaveAllChannels(void);

		void	setLastPing(time_t time);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
		void	setRealname(std::string realname);
		void	setQuitMessage(std::string quitMessage);

		time_t		getLastPing();
		int			getFd();
		std::string	getHost();
		std::string	getNickname();
		std::string	getUsername();
		std::string	getRealname();
		std::string	getUserModes();
		std::string	getQuitMessage();
		Server*		getServer();

		std::map<std::string, void (*)(Command*)>&	getCommands();

		int	status;

	private:
		time_t		_lastPing;
		int			_fd;
		std::string	_host;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_modes;
		std::string	_quitMessage;

		Server*										_server;
		std::map<std::string, Channel*>				_channels;

		std::map<std::string, void (*)(Command*)>	_commands;
};

# include "Server.hpp"
# include "Channel.hpp"

#endif
