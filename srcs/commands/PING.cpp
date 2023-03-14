#include "Command.hpp"
#include "Client.hpp"

void	PING(Command* command) {
	Client*	client = command->getClient();

	if (command->getValues().size() < 2) {
		client->sendReply(ERR_NOORIGIN());
		return ;
	}
	
	client->send(":" + client->getPrefix() + " PONG :" + command->getValues()[1]);
}

/* 
Numeric Replies:

	ERR_NOORIGIN
	ERR_NOSUCHSERVER -> We can ignore this error, because we don't have multy servers
*/

/* 
Example of server response:
	:xle-baux!xlebaux@localhost PONG :xle-baux
*/

/* 
Syntax:

	PING [<nick> | <channel> | *]

Parameters:

	The nickname or channel to ping, you can use the asterisk '*' character to
	ping every user on a channel.

Description:

	Sends a CTCP PING request to a nickname or a channel; this is used to find
	out the latency on the network.

Examples:

	/PING bob
	/PING #irssi
 */






