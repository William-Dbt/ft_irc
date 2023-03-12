#ifndef __COMMAND_HPP__
# define __COMMAND_HPP__

# include <string>
# include <vector>

class	Client;

class	Command {
	public:
		Command(Client* client, std::string line);
		~Command();

		void	execute();

	private:
		Client*						_client;
		std::string					_commandLine;
		std::vector<std::string>	_commandValues;
};

# include "Client.hpp"

#endif
