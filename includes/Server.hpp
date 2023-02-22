#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# include <poll.h>
# include <vector>
# include "Client.hpp"

# define ON			true
# define OFF		false
# define MAXCONNECT	2000
# define TIMEOUT	500

class	Server {
	public:
		Server();

		int	getStatus() const;

		int		init(char* const port);
		void	run();
		void	acceptClient();

	private:
		int						fd;
		int						status;
		std::vector<pollfd>	polls;
		std::vector<Client>		clients;
};

#endif
