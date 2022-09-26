//#ifndef IRCSERVERPROTOCOL
//# define IRCSERVERPROTOCOL

#define RPL_WELCOME(nickname)                           (":ircserv 001 " + (nickname) + " :Welcome to the IRCServ, " + (nickname))
#define RPL_MOTDSTART(nickname)                         (":ircserv 375 " + (nickname) + " :- Message of the day -")
#define RPL_MOTD(nickname, message)                     (":ircserv 372 " + (nickname) + " :- " + (message))


//#endif