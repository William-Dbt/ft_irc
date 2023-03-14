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

		Client*						getClient();
		std::string&				getLine();
		std::vector<std::string>&	getValues();

	private:
		Server*						_server;
		Client*						_client;
		std::string					_commandLine;
		std::vector<std::string>	_commandValues;
};

void	PASS(Command* command);
void	NICK(Command* command);
void	USER(Command* command);
void	MODE(Command* command);
void	PING(Command* command);
void	PONG(Command* command);
void	QUIT(Command* command);
void 	VERSION(Command* command);

# include "Client.hpp"
# include "Server.hpp"

#endif
