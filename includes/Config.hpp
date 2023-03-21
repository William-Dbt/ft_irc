#ifndef __CONFIG_HPP__
# define __CONFIG_HPP__

# define CONFIGFILE		"config/default.config"

// Minimal configuration ------------------
# define VAR_SERVERNAME		"server_name"
# define SERVERNAME			"TotIrc"

# define VAR_VERSION_SERVER	"version"
# define VERSION_SERVER		"1.0"

# define VAR_MOTDFILE		"motd_file"
# define MOTDFILE			"config/motd.txt"

# define VAR_MAXUSERS		"max_users"
# define MAXUSERS			"2000"

# define VAR_PINGDELAY		"ping_delay"
# define PINGDELAY			"50"

# define VAR_TIMEOUT		"timeout"
# define TIMEOUT			"300"

# define VAR_OPERUSERNAME	"oper_username"
# define OPERUSERNAME		"admin"

# define VAR_OPERPASSWORD	"oper_password"
# define OPERPASSWORD		"admin"

# define TOTALCONFIGS		8
// ----------------------------------------

/* 
# User mods (the respective config is call "user_mods")
# a - user is flagged as away
# i - marks a users as invisible
# w - user receives wallops
# r - restricted user connection
# o - operator flag
# s - marks a user for receipt of server notices. // Useless but still supported
#
# Channel mods (the respective config is call "channel_mods")
# O - give "channel creator" status
# o - give/take channel operator privilege
# v - give/take the voice privilege
#
# a - toggle the anonymous channel flag
# i - toggle the invite-only channel flag
# m - toggle the moderated channel
# n - toggle the no messages to channel from clients on the outside
# q - toggle the quiet channel flag
# p - toggle the private channel flag
# s - toggle the secret channel flag
# r - toggle the server reop channel flag
# t - toggle the topic settable by channel operator only flag
#
# k - set/remove the channel key (password)
# l - set/remove the user limit to channel
# b - set/remove ban mask to keep users out
# e - set/remove an exception mask to override a ban mask
# I - set/remove an invitation mask to automatically override the invite-only flag
*/

# include <map>
# include <vector>
# include <string>

class	Config {
	public:
		Config();
		~Config();

		void	error(std::string error);

		void	checkMinimumConfig();
		void	setDefaultConfig();
		void	setFileConfig();

		std::string	get(std::string configName);
		void		set(std::string configName, std::string value);

	private:
		std::map<std::string, std::string>	_config;
		std::vector<std::string>			_configNames;
};

#endif
