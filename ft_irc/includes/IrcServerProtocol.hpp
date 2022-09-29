#ifndef IRCSERVERPROTOCOL
# define IRCSERVERPROTOCOL

# define RPL_WELCOME(nickname)                           (":ircserv 001 " + (nickname) + " :Welcome to the IRCServ, " + (nickname))
# define RPL_MOTD(nickname, message)                     (":ircserv 372 " + (nickname) + " :- " + (message))
# define RPL_MOTDSTART(nickname)                         (":ircserv 375 " + (nickname) + " :- Message of the day -")
# define RPL_ENDOFMOTD(nickname)                         (":ircserv 376 " + (nickname) + " :- End of /MOTD command") // После этого сообщения Adium загорается зеленый




# define RPL_YOUREOPER(nickname)                         (":ircserv 381 " + (nickname) + " :You are now an IRC operator")



//PASS
#define ERR_NOTREGISTERED(nickname)                     (":ircserv 451 " + (nickname) + " :You have not registered")
#define ERR_NEEDMOREPARAMS(nickname, command)           (":ircserv 461 " + (nickname) + ' ' + (command) + " :Not enough parameters")
#define ERR_ALREADYREGISTRED(nickname)                  (":ircserv 462 " + (nickname) + " :You may not reregister")
#define ERR_PASSWDMISMATCH(nickname)                    (":ircserv 464 " + (nickname) + " :Password incorrect")


//NICK
#define ERR_NONICKNAMEGIVEN(nickname)                   (":ircserv 431 " + (nickname) + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(nickname, nick)            (":ircserv 432 " + (nickname) + ' ' + (nick) + " :Erroneus nickname")
#define ERR_NICKNAMEINUSE(oldnickname, nickname)        (":ircserv 433 " + (oldnickname) + ' ' + (nickname) + " :Nickname is already in use")


//__ERRORS_____________________________________________________
#define ERR_NOSUCHNICK(nickname, nick)                  (":ircserv 401 " + (nickname) + ' ' + (nick) + " :No such nick/channel")
#define ERR_NOSUCHCHANNEL(nickname, channel)            (":ircserv 403 " + (nickname) + ' ' + (channel) + " :No such channel")
#define ERR_NOORIGIN(nickname)                          (":ircserv 409 " + (nickname) + " :No origin specified")
#define ERR_CANNOTSENDTOCHAN(channel)                   (":ircserv 404 " + (channel) + " :Cannot send to channel")
#define ERR_WASNOSUCHNICK(channel)                      (":ircserv 406 " + (channel) + " :There was no such nickname")
#define ERR_TOOMANYTARGETS(nickname, target)            (":ircserv 407 " + (nickname) + ' ' + (target) + " :Duplicate recipients. No message delivered")
#define ERR_UNKNOWNCOMMAND(nickname, command)           (":ircserv 421 " + (nickname) + ' ' + (command) + " :Unknown command")
#define ERR_CHANOPRIVSNEEDED(nickname, channel)         (":ircserv 482 " + (nickname) + ' ' + (channel) + " :You're not channel operator")


#define RPL_QUIT(nickname, message)                     (":ircserv " + (nickname) + " quited :" + (message))

#endif