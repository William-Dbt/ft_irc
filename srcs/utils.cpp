#include <string>
#include <ctime>
#include "utils.hpp"

const std::string	getCurrentDateTime(bool both, bool onlyDate) {
	time_t		now = time(NULL);
	struct tm	timeStruct;
	char		buffer[255];

	timeStruct = *localtime(&now);
	if (both)
		strftime(buffer, sizeof(buffer), "%B %e, %Y %r", &timeStruct);
	else if (onlyDate)
		strftime(buffer, sizeof(buffer), "%B %e, %Y", &timeStruct);
	else
		strftime(buffer, sizeof(buffer), "%r", &timeStruct);

	return buffer;
}
