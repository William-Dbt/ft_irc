#include <string>
#include <ctime>
#include "utils.hpp"

std::string	getCurrentDateTime(bool both, bool onlyDate) {
	time_t		now = time(NULL);
	struct tm	timeStruct;
	char		buffer[255];

	timeStruct = *localtime(&now);
	if (both)
		strftime(buffer, sizeof(buffer), "%B %e, %Y %r", &timeStruct);
	else if (onlyDate)
		strftime(buffer, sizeof(buffer), "%B %e, %Y", &timeStruct);
	else
		strftime(buffer, sizeof(buffer), "%T", &timeStruct);

	return buffer;
}

std::string	intToString(int value) {
	std::ostringstream	ssbuffer;
	std::string			buffer;

	ssbuffer << value;
	buffer = ssbuffer.str();
	return buffer;
}

// user_modes = a i w r o s
bool	isValidUserMode(char c) {
	if (c == 'a' || c == 'i' || c == 'w' || c == 'r' || c == 'o' || c == 's')
		return true;

	return false;
}

// special = %x5B-60 / %x7B-7D ; "[", "]", "\", "`", "_", "^", "{", "|", "}"
bool	isSpecial(char c) {
	if (c == '[' || c == ']' || c == '\\' || c == '`'
		|| c == '_' || c == '^' || c == '{' || c == '|' || c == '}')
		return true;

	return false;
}

// letter = %x41-5A / %x61-7A ; A-Z / a-z
bool	isLetter(char c) {
	if (c >= 'A' && c <= 'Z')
		return true;

	if (c >= 'a' && c <= 'z')
		return true;

	return false;
}

// digit = %x30-39 ; 0-9
bool	isDigit(char c) {
	if (c >= '0' && c <= '9')
		return true;

	return false;
}

void	printConfigLog(std::string log) {
	std::string buffer = KBOLD;

	buffer += KGRAY + getCurrentDateTime(false, false);
	buffer += ":";
	buffer += KWHT;
	buffer += "CONFIG";
	buffer += KGRAY;
	buffer += "{*}"; 
	buffer += KWHT;
	buffer += "[";
	buffer += KRESET;
	buffer += KBLU;
	buffer += log + KRESET;
	std::cout << buffer << std::endl;
}

void	printServerLog(std::string log) {
	std::string buffer = KBOLD;

	buffer += KGRAY + getCurrentDateTime(false, false);
	buffer += ":";
	buffer += KWHT;
	buffer += "SERVER";
	buffer += KGRAY;
	buffer += "{*}";
	buffer += KWHT;
	buffer += "[";
	buffer += KRESET;
	buffer += KYEL;
	buffer += log + KRESET;
	std::cout << buffer << std::endl;
}

void	printServerLog(int fd, std::string log, bool received) {
	std::string	buffer = KBOLD;

	buffer += KGRAY + getCurrentDateTime(false, false);
	buffer += ":";
	buffer += KWHT;
	if (received)
		buffer += "   <--";
	else
		buffer += "-->   ";

	buffer += KGRAY;
	buffer += "{" + intToString(fd) + "}";
	buffer += KWHT;
	buffer += "[";
	buffer += KRESET;
	if (received)
		buffer += KMAG;
	else
		buffer += KCYN;

	buffer += log + KRESET;
	std::cout << buffer;
}
