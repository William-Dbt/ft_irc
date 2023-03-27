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

std::string	charToString(char c) {
	std::string	buffer;

	buffer.append(1, c);
	return buffer;
}

// user_modes = a i w r o s
bool	isValidUserMode(char c, Config* config) {
	if (config->get("user_mods").find(c) != std::string::npos)
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

static std::string	getLogInfo(int prefix, int infoOfColor) {
	switch (prefix) {
		case RECEIVED: {
			if (infoOfColor)
				return KMAG;

			return ("   <--");
		}
		case SENDING: {
			if (infoOfColor)
				return KCYN;

			return ("-->   ");
		}
		case SERVER: {
			if (infoOfColor)
				return KYEL;

			return ("SERVER");
		}
		case CONFIG: {
			if (infoOfColor)
				return KBLU;

			return ("CONFIG");
		}
		default: {
			if (infoOfColor)
				return KRESET;

			return ("      ");
		}
	};
}

void	printLog(std::string log, int prefix, int fd) {
	std::string	buffer = KBOLD;

	buffer.append(KGRAY + getCurrentDateTime(false, false));
	buffer.append(":");
	buffer.append(KWHT);
	buffer.append(getLogInfo(prefix, false));
	buffer.append(KGRAY);
	if (fd != -1)
		buffer.append("{" + intToString(fd) + "}");
	else
		buffer.append("{*}");

	buffer.append(KWHT);
	buffer.append("[");
	buffer.append(KRESET);
	buffer.append(getLogInfo(prefix, true));
	buffer.append(log + KRESET);
	if (prefix == SERVER || prefix == CONFIG)
		std::cout << buffer << std::endl;
	else
		std::cout << buffer;
}
