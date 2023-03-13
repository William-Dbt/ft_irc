#ifndef __REPLIES_HPP__
# define __REPLIES_HPP__

# define RPL_WELCOME(nickname, username, host) "001:Welcome to the Internet Relay Network " + nickname + "!" + username + "@" + host
# define RPL_YOURHOST(servername, version) "002:Your host is " + servername + ", running version " + version
# define RPL_CREATED(date) "003:This server was created " + date
# define RPL_MYINFO(servername, version, usermodes, channelmodes) "004:" + servername + " " + version + " " + usermodes + " " + channelmodes

# define ERR_NOSUCHNICK(nickname) "401" + nickname + " :No such nick/channel"
# define ERR_NOSUCHSERVER(servername) "402" + servername + " :No such server"
# define ERR_NOSUCHCHANNEL(channelname) "403" + channelname + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(channelname) "404" + channelname + " :Cannot send to channel"
# define ERR_TOOMANYCHANNELS(channelname) "405" + channelname + " :You have joined too many channels"
# define ERR_TOOMANYTARGETS(target) "407" + target + " :Duplicate recipients. No message delivered"
# define ERR_NOORIGIN() "409:No origin specified"
# define ERR_NORECIPIENT(command) "411:No recipient given (" + command + ")"
# define ERR_NOTOPLEVEL(mask) "413" + mask + " :No toplevel domain specified"
# define ERR_WILDTOPLEVEL(mask) "414" + mask + " :Wildcard in toplevel domain"
# define ERR_NOTEXTTOSEND() "412:No text to send"
# define ERR_NOMOTD() "422:MOTD File is missing"
# define ERR_NONICKNAMEGIVEN() "431:No nickname given"
# define ERR_ERRONEUSNICKNAME(nickname) "432" + nickname + " :Erroneous nickname"
# define ERR_NICKNAMEINUSE(nickname) "433" + nickname + " :Nickname is already in use"
# define ERR_NICKCOLLISION(nickname, username, host) "436" + nickname + " :Nickname collision KILL from " + username + "@" + host
# define ERR_UNAVAILRESOURCE(nickchannel) "437" + nickchannel + " :Nick/channel is temporarily unavailable"
# define ERR_USERNOTINCHANNEL(nickname, channel) "441" + nickname + " " + channel + " :They aren't on that channel"
# define ERR_NOTONCHANNEL(channel) "442" + channel + " :You're not on that channel"
# define ERR_USERONCHANNEL(username, channel) "443" + username + " " + channel + " :is already on channel"
# define ERR_NEEDMOREPARAMS(command) "461" + command + " :Not enough parameters"
# define ERR_ALREADYREGISTRED() "462:Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH() "464:Password incorrect"
# define ERR_CHANNELISFULL(channel) "471" + channel + " :Cannot join channel (+l)"
# define ERR_INVITEONLYCHAN(channel) "473" + channel + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(channel) "474" + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(channel) "475" + channel + " :Cannot join channel (+k)"
# define ERR_BADCHANMASK(channel) "476" + channel + " :Bad Channel Mask"
# define ERR_NOCHANMODES(channel) "477" + channel + " :Channel doesn't support modes"
# define ERR_NOPRIVILEGES() "481:Permission Denied- You're not an IRC operator"
# define ERR_CHANOPRIVSNEEDED(channel) "482" + channel + " :You're not channel operator"
# define ERR_CANTKILLSERVER() "483:You can't kill a server!"
# define ERR_RESTRICTED() "484:Your connection is restricted!"
# define ERR_NOOPERHOST() "491:No O-lines for your host"
# define ERR_UMODEUNKNOWNFLAG() "501:Unknown MODE flag"
# define ERR_USERSDONTMATCH() "502:Cannot change mode for other users"

# define RPL_REHASHING(configfile) "382" + configfile + " :Rehashing"
# define RPL_VERSION(version, debuglevel, server, comments) "351" + version + "." + debuglevel + " " + server + " :" + comments
# define RPL_MOTDSTART(server) "375:- " + server + " Message of the day - "
# define RPL_MOTD(text) "372:- " + text
# define RPL_ENDOFMOTD() "376:End of MOTD command"
# define RPL_AWAY(nick, message) "301" + nick + " :" + message
# define RPL_INVITING(channel, nick) "341" + channel + " " + nick
# define RPL_NOTOPIC(channel) "331" + channel + " :No topic is set"
# define RPL_TOPIC(channel, topic) "332" + channel + " :" + topic
# define RPL_UMODEIS(usermode) "221" + usermode
# define RPL_YOUREOPER() "381:You are now an IRC operator"


#endif

