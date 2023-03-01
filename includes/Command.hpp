#ifndef __COMMAND_HPP__
# define __COMMAND_HPP__

#include <string>
#include <vector>
#include "Client.hpp"


#define KRESET "\x1B[0m"
#define KBLK  "\x1B[30m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"


void reply (unsigned short code, Client &client, std::vector<std::string> params);


# endif