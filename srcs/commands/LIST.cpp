#include "Command.hpp"

void LIST(Command *command)
{
	(void)command;
}







/*
Command: LIST
Parameters: [ <channel> *( "," <channel> ) [ <target> ] ]

ERR_TOOMANYMATCHES

ERR_NOSUCHSERVER =	Used to indicate the server name given currently does not exist.
							"402" + servername + " :No such server"	



					Replies RPL_LIST, RPL_LISTEND mark the actual replies
					with data and end of the server's response to a LIST
					command.  If there are no channels available to return,
					only the end reply MUST be sent.
RPL_LIST = "<channel> <# visible> :<topic>"
RPL_LISTEND





LIST
; Command to list all channels.

LIST #twilight_zone,#42
; Command to list channels #twilight_zone and #42

*/