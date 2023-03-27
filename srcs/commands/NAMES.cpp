#include "Command.hpp"

void NAMES(Command *command)
{
	(void)command;
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