#ifndef __UTILS_HPP__
# define __UTILS_HPP__

# include <string>

const std::string	getCurrentDateTime(bool both = true, bool onlyDate = false);

bool	isSpecial(char c);
bool	isLetter(char c);
bool	isDigit(char c);

#endif
