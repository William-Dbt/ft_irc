#ifndef __UTILS_HPP__
# define __UTILS_HPP__

# define KRESET		"\x1B[0m"
# define KBOLD		"\x1b[1m"
# define KGRAY		"\x1B[90m"
# define KBLK		"\x1B[30m"
# define KRED		"\x1B[31m"
# define KGRN		"\x1B[32m"
# define KYEL		"\x1B[33m"
# define KBLU		"\x1B[94m"
# define KMAG		"\x1B[35m"
# define KCYN		"\x1B[36m"
# define KWHT		"\x1B[97m"

# include <string>
# include "Client.hpp"
# include "Config.hpp"

enum	e_prefix {
	RECEIVED = 0,
	SENDING,
	SERVER,
	CONFIG
};

std::string	getCurrentDateTime(bool both = true, bool onlyDate = false);
std::string	intToString(int value);

bool	isValidUserMode(char c, Config* config);
bool	isSpecial(char c);
bool	isLetter(char c);
bool	isDigit(char c);

void	printLog(std::string log, int prefix, int fd = -1);

#endif
