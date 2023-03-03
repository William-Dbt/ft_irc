#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# define CONFIGFILE		"config/default.config"

// ----------------------------------------
# define VAR_SERVERNAME	"server_name"
# define SERVERNAME		"TotIrc"

# define VAR_VERSION	"version"
# define VERSION		"1.0"

# define VAR_MOTDFILE	"motd_file"
# define MOTDFILE		"config/motd.txt"

# define VAR_MAXUSERS	"max_users"
# define MAXUSERS		"4000"

# define VAR_TIMEOUT	"timeout"
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

		std::string	get(std::string configName);

	private:
		std::map<std::string, std::string>	_config;
		std::vector<std::string>			_configNames;
};

#endif
