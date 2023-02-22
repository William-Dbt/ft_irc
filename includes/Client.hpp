#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

class	Client {
	public:
		Client(int const &fd);

		int	getFd() const;

	private:
		int	fd;
};

#endif
