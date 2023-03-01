#include <string>
#include <sstream>
#include <vector>
#include <iostream>
// #include "Client.hpp"


std::string RPL_WELCOME(std::string prefix) { return ("Welcome to the Internet Relay Network " + prefix); }
std::string RPL_YOURHOST(std::string servername, std::string ver) { return ("Your host is " + servername + ", running version " + ver); }


// void reply (unsigned short code, Client &client, std::vector<std::string> params)
// {
// 	std::ostringstream	sscode;
// 	std::string			scode;
// 	(void)params;

// 	sscode << code;
// 	scode = sscode.str();

// 	std::string reply = ":" + client.getPrefix() + " " + scode + " " + client.getNickname() + " :";
// 	// switch (code)
// 	// {
// 	// 	case 1:
// 	// 		reply += RPL_WELCOME(this->getPrefix());
// 	// 		break;
// 	// 	case 2:
// 	// 		reply += RPL_YOURHOST();
// 	// 		break;
// 	// }
// }
// {x


