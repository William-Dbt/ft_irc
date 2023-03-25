#include "Command.hpp"
#include "Channel.hpp"

void PRIVMSG(Command *command)
{
	Client		*senderClient = command->getClient();
	Server		*server = senderClient->getServer();
	std::string	message = command->getEndParam();
	std::string	target = command->getParameters()[1];

	if (target[0] == '#')
	{
		Channel *channel = server->getChannel(target);
		
		std::map<int, Client *> clients = channel->getClients();
		std::map<int, Client *>::iterator it;

		for (it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it).second != senderClient)
				(*it).second->sendFrom(senderClient, "PRIVMSG " + target + " :" + message);

		}
	}
	else
	{
		Client *targetClient = server->getClient(target);
		if (targetClient == NULL)
			return senderClient->sendReply(ERR_NOSUCHNICK(target));
		targetClient->sendFrom(senderClient, "PRIVMSG " + target + " :" + message);
	}


}

/*

Command: PRIVMSG
Parameters: <msgtarget> <text to be sent>

ERR_NOSUCHNICK =	Used to indicate the nickname parameter supplied to a command
					is currently unused.
						"401" + nickname + " :No such nick/channel"

						Are returned by PRIVMSG to indicate that
						the message wasn't delivered for some reason.
						ERR_NOTOPLEVEL and ERR_WILDTOPLEVEL are errors that
						are returned when an invalid use of
						"PRIVMSG $<server>" or "PRIVMSG #<host>" is attempted.
ERR_NORECIPIENT				= "411:No recipient given (" + command + ")"
ERR_NOTEXTTOSEND			= "412:No text to send"
ERR_NOTOPLEVEL				= "413" + mask + " :No toplevel domain specified"
ERR_WILDTOPLEVEL			= "414" + mask + " :Wildcard in toplevel domain"

ERR_TOOMANYTARGETS =	Returned to a client which is attempting to send a
						PRIVMSG/NOTICE using the user@host destination format
						and for a user@host which has several occurrences.

						Returned to a client which trying to send a
						PRIVMSG/NOTICE to too many recipients.

						Returned to a client which is attempting to JOIN a safe
						channel using the shortname when there are more than one
						such channel.
							"407" + target + " :Duplicate recipients. No message delivered"

ERR_CANNOTSENDTOCHAN =	Sent to a user who is either (a) not on a channel
						which is mode +n or (b) not a chanop (or mode +v) on
						a channel which has mode +m set or where the user is
						banned and is trying to send a PRIVMSG message to
						that channel.
							"404" + channel + " :Cannot send to channel"

RPL_AWAY =	These replies are used with the AWAY command (if
			allowed).  RPL_AWAY is sent to any client sending a
			PRIVMSG to a client which is away.  RPL_AWAY is only
			sent by the server to which the client is connected.
			Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the
			client removes and sets an AWAY message.
				 "<nick> :<away message>"






:Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
; Message from Angel to Wiz.

PRIVMSG Angel :yes I'm receiving it !
; Command to send a message to Angel.

PRIVMSG jto@tolsun.oulu.fi :Hello !
; Command to send a message to a user on server tolsun.oulu.fi with username of "jto".

PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
; Message to a user on server irc.stealth.net with username of "kalt", and connected from the host millennium.stealth.net.

PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
; Message to a user on the local server with username of "kalt", and connected from the host millennium.stealth.net.

PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
; Message to the user with nickname Wiz who is connected from the host tolsun.oulu.fi and has the username "jto".

PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
; Message to everyone on a server which has a name matching *.fi.

PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
; Message to all users who come from a host which has a name matching *.edu.

*/