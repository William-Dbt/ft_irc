# 🖱 42Paris Project | ft_irc

## Objectives

The goal of this project is to make an **IRC Server** using an actual IRC Client as reference. (For this project we used [irssi](https://irssi.org/) client)  
We only have to implement a server that supports connection of **multiple users** and the **communication** between us with channels.  
The subject asks the following things to implement:  
- You must be able to **authenticate**, set a **nickname**, a **username**, **join** a channel, **send** and **receive** private **messages** using your reference client.  
- All the **messages** sent from one **client** to a **channel** have to be **forwarded** to **every other client** that joined the channel.  
- You must have **operators** and **regular users**.  
- Then, you have to implement the **commands** that are specific to **operators**.  

The following client's commands are supported:  
- **AWAY**: set the away mode for the client.  
- **DIE**: disconnect the server (client have to be an operator).  
- **INVITE**: invite someone on a channel (have to be a channel operator).  
- **JOIN**: join a channel (it create one if doesn't exists).  
- **KICK**: kick someone from a channel.  
- **KILL**:: kick someone from the server (have to be an operator).  
- **LIST**: list all channels available.  
- **MODE**: change/print modes for a user.  
- **MOTD**: print the message of the day of the server.  
- **NAMES**: list all users that are visible in a channel.  
- **NICK**: change nickname.  
- **NOTICE**: send message to all users that are connected to the server.  
- **OPER**: set user to operator (have to connect with a login and password defined in config file).  
- **PART**: leave a channel.  
- **PASS**: this command is managed by irssi when a user connect to a server but with nc (for example), you can enter this command manually to set the password of the server.  
- **PING**: this command send a ping to the client to check if the connection between the server and the client is still available.  
- **PONG**: this command is the answer of PING.  
- **PRIVMSG**: send a private message to someone.  
- **QUIT**: leave the server.  
- **REHASH**: reload the configuration file.  
- **TOPIC**: set a topic to a channel.  
- **USER**: this command is managed by irssi when a user connect to a server, this commands get the Realname and the Username of the user.  
- **VERSION**: get the version of the server.  
- **WALLOPS**: send message to all users that are connected to the server.  

The mods managed by our server for users is: **aiwros**  
- **a**: user is flagged as away
- **i**: marks a users as invisible
- **w**: user receives wallops
- **r**: restricted user connection
- **o**: operator flag
- **s**: marks a user for receipt of server notices

and for channel it's only the flag '**i**' (to invite-only mode) and the flag '**o**' wich is given to the channel creator.  

## Usage

`make` to compile the program.  
`clean` to clean objects files.  
`fclean` to clean objects files and program file.  

**Execute**: `./ircserv <port> <password>`  
The **port** refers to the port wich is listening and the **password** is the password of the server (it can be optional (not a requirement of the subject))  

To connect to the server, use the irssi program and in the command line just type `/connect localhost <port> <password>`  

Tips for the bonus (file transfert): [DCC Command](https://irssi.org/documentation/help/dcc/), irssi already managed the file transfert by the PRIVMSG command.

## Help

[Unix Socket Tutorial](https://www.tutorialspoint.com/unix_sockets/index.htm)  
[Socket Programming in C/C++](https://www.geeksforgeeks.org/socket-programming-cc/)  
[RFC2811](https://datatracker.ietf.org/doc/html/rfc2811)  
[RFC2812](https://datatracker.ietf.org/doc/html/rfc2812)  
[Simple example of a server in C](https://codes-sources.commentcamarche.net/source/36603-server-irc-en-c-protocole-maison)

## Score

[![wdebotte's 42 ft_irc Score](https://badge42.vercel.app/api/v2/cl2zu1sil002509mf9zd91hy6/project/2974353)](https://github.com/JaeSeoKim/badge42)

Done with [Xavier Le-baux](https://github.com/Xavier-LB) and [Nicolas Mattera](https://github.com/niko-mttr).
