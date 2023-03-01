#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# define CONFIGFILE		"config/default.config"

// ----------------------------------------
# define NSERVERNAME	"server_name"
# define SERVERNAME		"TotIrc"

# define NVERSION		"version"
# define VERSION		"1.0"

# define NMOTDFILE		"motd_file"
# define MOTDFILE		"config/motd.txt"

# define NMAXUSERS		"max_users"
# define MAXUSERS		"4000"

# define NTIMEOUT		"timeout"
# define TIMEOUT		"300"
// ----------------------------------------

# define TOTALCONFIGS	5

# include <map>
# include <vector>
# include <string>

class	Config {
	public:
		Config();
		~Config();

		void	error(std::string error);

		bool	checkMinimumConfig();
		void	setDefaultConfig();
		void	setFileConfig();

	private:
		std::map<std::string, std::string>	_config;
		std::vector<std::string>			_configNames;
};

#endif
