#ifndef __COMMAND_HPP__
# define __COMMAND_HPP__

# include <string>
# include <vector>

class	Client;
class	Server;

class	Command {
	public:
		Command(Client* client, std::string line);
		~Command();

		void		execute();
		std::string	getCommandLine();

	private:
		Server*						_server;
		Client*						_client;
		std::string					_commandLine;
		std::vector<std::string>	_commandValues;
};

void	MODE(Command* command);
void	PING(Command* command);
void	QUIT(Command* command);

# include "Client.hpp"
# include "Server.hpp"

#endif
