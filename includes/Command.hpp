#ifndef __COMMAND_HPP__
# define __COMMAND_HPP__

# include <string>
# include <vector>
# include <map>
# include "replies.hpp"

class	Client;
class	Server;

class	Command {
	public:
		Command(Client* client, std::string line);
		~Command();

		void	execute();

		Server*						getServer();
		Client*						getClient();
		std::string					getLine();
		std::string					getEndParam();
		std::vector<std::string>	getParameters();

	private:
		Server*						_server;
		Client*						_client;
		std::string					_line;
		std::vector<std::string>	_parameters;
};

void	PASS(Command* command);
void	NICK(Command* command);
void	USER(Command* command);
void	OPER(Command* command);
void	MODE(Command* command);
void	QUIT(Command* command);
void	JOIN(Command* command);
// void	PART(Command* command);
void	TOPIC(Command* command);
void	INVITE(Command* command);
void	KICK(Command* command);
void	PRIVMSG(Command* command);
void	MOTD(Command* command);
void	VERSION(Command* command);
void	KILL(Command* command);
void	PING(Command* command);
void	PONG(Command* command);
void	REHASH(Command* command);
void	DIE(Command* command);
void	WALLOPS(Command* command);

# include "Client.hpp"
# include "Server.hpp"

#endif
