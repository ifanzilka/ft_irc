//#ifndef IRCSERVERPROTOCOL
//# define IRCSERVERPROTOCOL

#define RPL_WELCOME(nickname)                           (":ircserv 001 " + (nickname) + " :Welcome to the IRCServ, " + (nickname))
#define RPL_MOTD(nickname, message)                     (":ircserv 372 " + (nickname) + " :- " + (message))
#define RPL_MOTDSTART(nickname)                         (":ircserv 375 " + (nickname) + " :- Message of the day -")
#define RPL_ENDOFMOTD(nickname)                         (":ircserv 376 " + (nickname) + " :- End of /MOTD command") // После этого сообщения Adium загорается зеленый




#define RPL_YOUREOPER(nickname)                         (":ircserv 381 " + (nickname) + " :You are now an IRC operator")

//#endif