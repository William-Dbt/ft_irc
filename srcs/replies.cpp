#include "Command.hpp"


std::string RPL_WELCOME(std::string prefix) { return ("Welcome to the Internet Relay Network " + prefix); }
// std::string RPL_YOURHOST(std::string servername, std::string ver) { return ("Your host is " + servername + ", running version " + ver); }


void reply (unsigned short code, Client &client, std::vector<std::string> params)
{
	std::ostringstream	sscode;
	std::string			scode;
	(void)params;

	sscode << code;
	scode = sscode.str();

	std::string reply = ":" + client.getPrefix() + " " + scode + " " + client.getNickname() + " :";
	std::cout << KBLU << reply << KRESET << std::endl;
	// switch (code)
	// {
	// 	case 1:
	// 	{
	// 		reply += RPL_WELCOME(client.getPrefix());
	// 		break;
	// 	}
	// 	// case 2:
	// 	// {
	// 	// 	reply += RPL_YOURHOST("TotIrc", "0.1");
	// 	// 	break;
	// 	// }
	// 	// case 2:
	// 	// 	reply += RPL_YOURHOST();
	// 	// 	break;
	// }
	// std::cout << KRED << "TOTOTOT" << KRESET << std::endl;
	send(client.getFd(), reply.c_str(), reply.size(), 0);
}
