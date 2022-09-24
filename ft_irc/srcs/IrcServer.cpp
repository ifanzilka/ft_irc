#ifndef IRCSERVER
# define IRCSERVER

#include <IrcServer.hpp>

/* Constructos */

IrcServer::IrcServer(int port, std::string password)
{
    std::string tmp = "127.0.0.1";
            
    IrcServer::InitIRC(password, tmp, port);

}


IrcServer::IrcServer(std::string &ipaddr, int port, std::string password)
{
    IrcServer::InitIRC(password, ipaddr, port);
}


IrcServer::IrcServer(const char *ipaddr, int port,std::string password)
{
    std::string tmp = std::string(ipaddr);;

    IrcServer::InitIRC(password, tmp, port);
}

/* Destructor */

IrcServer::~IrcServer()
{
    delete _MainServer;
}

void IrcServer::InitComands()
{
    _commands["PASS"] = &IrcServer::pass;
}


void IrcServer::InitIRC(std::string password, std::string ipaddr, int port)
{

    // _MainServer = new ServerSelect(ipaddr, port);
    //_MainServer = new ServerPoll(ipaddr, port);
    // _MainServer = new ServerKqueue(ipaddr, port);
    // _MainServer = new ServerEpoll(ipaddr, port);

    #if defined(SELECT)
        _MainServer = new ServerSelect(ipaddr, port);
        _MainServer->Logger(GREEN, std::string("Server Type Select"));
    #elif defined(POLL)
        _MainServer = new ServerPoll(ipaddr, port);
        _MainServer->Logger(GREEN, std::string("Server Type Poll"));
    #elif defined(EPOLL)
        _MainServer = new ServerEpoll(ipaddr, port);
        _MainServer->Logger(GREEN, std::string("Server Type Epoll"));
    #else
        _MainServer = new ServerKqueue(ipaddr, port);
        _MainServer->Logger(GREEN, std::string("Server Type Kqueue"));
    #endif

    
    InitComands();
    _pass = password;
}


# endif