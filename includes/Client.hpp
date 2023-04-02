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
	BADNICKNAME,
	COMMING,
	REGISTER,
	FULLYREGISTER,
	CONNECTED,
	DISCONNECTED
};

class	Client {
	public:
		Client(const int& fd, const std::string& host, Server* server);
		~Client();

		void	connectToClient(Server &server);

		void	send(std::string message);
		void	sendTo(std::string message);
		void	sendFrom(Client * senderClient, std::string message);
		void	sendReply(std::string message);

		std::string	getPrefix();
		bool		isModeInUse(char mode);
		void		addMode(char mode);
		void		removeMode(char mode);

		void	setLastPing(time_t time);
		void	setNickname(std::string nickname);
		void	setUsername(std::string username);
		void	setRealname(std::string realname);
		void	setQuitMessage(std::string quitMessage);
		void	setAwayMessage(std::string awayMessage);

		time_t		getLastPing();
		int			getFd();
		std::string	getHost();
		std::string	getNickname();
		std::string	getUsername();
		std::string	getRealname();
		std::string	getUserModes();
		std::string	getQuitMessage();
		std::string	getAwayMessage();
		Server*		getServer();

		std::map<std::string, void (*)(Command*)>&	getCommands();

		int			status;
		std::string	commandBuffer;

	private:
		time_t		_lastPing;
		int			_fd;
		std::string	_host;
		std::string	_nickname;
		std::string	_username;
		std::string	_realname;
		std::string	_modes;
		std::string	_quitMessage;
		std::string	_awayMessage;

		Server*										_server;
		std::map<std::string, void (*)(Command*)>	_commands;
};

# include "Server.hpp"

#endif
