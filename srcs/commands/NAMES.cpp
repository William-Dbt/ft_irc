#include "Command.hpp"

void NAMES(Command *command)
{
	std::vector<std::string>	channels;
	Channel*					channel;

	if (command->getParameters().size() == 3)
		return command->getClient()->sendReply(ERR_NOSUCHSERVER(command->getParameters()[2]));

	channels = command->multipleParams(command->getParameters()[1]);

	while (channels.size() > 0)
	{
		if (command->getServer()->getChannel(channels[0]) == NULL)
			command->getClient()->sendReply(RPL_ENDOFNAMES(channels[0]));
		else
		{
			channel = command->getServer()->getChannel(channels[0]);
			command->getClient()->sendReply(RPL_NAMREPLY(channels[0], channel->getClientsNicknames()));
			command->getClient()->sendReply(RPL_ENDOFNAMES(channels[0]));
		}
		channels.erase(channels.begin());
	}
}


/*

Command: NAMES
Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]


ERR_TOOMANYMATCHES
ERR_NOSUCHSERVER =	Used to indicate the server name given currently does not exist.
							"402" + servername + " :No such server"	

RPL_NAMREPLY =	"( "=" / "*" / "@" ) <channel>
				:[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )

				"@" is used for secret channels, "*" for private
				channels, and "=" for others (public channels).
					"353= " + channel + " :" + users

	
RPL_ENDOFNAMES =	To reply to a NAMES message, a reply pair consisting
					of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
					server back to the client.  If there is no channel
					found as in the query, then only RPL_ENDOFNAMES is
					returned.  The exception to this is when a NAMES
					message is sent with no parameters and all visible
					channels and contents are sent back in a series of
					RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
					the end.
						"366" + channel + " :End of /NAMES list"









NAMES #twilight_zone,#42
; Command to list visible users on #twilight_zone and #42

NAMES
; Command to list all visible channels and users


*/