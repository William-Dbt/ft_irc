#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

class	Client {
	public:
		Client();
		~Client();

	private:
		enum	status {
			COMMING = 0,
			REGISTER,
			CONNECTED,
			DISCONNECTED
		};

		int	status;
		int	fd;
		std::string	host;
		std::string	password;
		std::string	nickname;
		std::string	username;
		std::string	realname;
};

#endif
