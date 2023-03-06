#include "Server.hpp"

std::string RPL_WELCOME(std::string prefix) { return ("Welcome to the Internet Relay Network " + prefix); }
std::string RPL_YOURHOST(std::string servername, std::string ver) { return ("Your host is " + servername + ", running version " + ver); }
std::string RPL_CREATED(std::string date) { return "This server was created " + date; }
std::string RPL_MYINFO(std::string servername, std::string version, std::string umodes, std::string cmodes) { return servername + " " + version + " " + umodes + " " + cmodes; }



std::string getReply(unsigned short code, std::string arg1, std::string arg2, std::string arg3, std::string arg4)
{
	switch (code)
	{
		case 1:
			return (RPL_WELCOME(arg1));
		case 2:
			return (RPL_YOURHOST(arg1, arg2));
		case 3:
			return (RPL_CREATED(arg1));
		case 4:
			return (RPL_MYINFO(arg1, arg2, arg3, arg4));

		default:
			return std::string();
	}
}

void reply (unsigned short code, Client &client, std::string arg1, std::string arg2, std::string arg3, std::string arg4)
{
	std::ostringstream	sscode;
	std::string			scode;

	sscode << code;
	scode = sscode.str();
	while (scode.size() < 3)
		scode = "0" + scode;

	std::string reply = ":" + client.getPrefix() + " " + scode + " " + client.getNickname() + " :";
	reply += KCYN;
	reply += getReply(code, arg1, arg2, arg3, arg4) /* + "\n" */;
	reply += KRESET;
	reply += "\n";

	// std::cout << KBLU << reply << KRESET << std::endl;
	send(client.getFd(), reply.c_str(), reply.size(), 0);
}


