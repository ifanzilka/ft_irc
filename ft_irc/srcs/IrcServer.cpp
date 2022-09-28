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
    _commands["PASS"] = &IrcServer::PASS;
    _commands["USER"] = &IrcServer::USER;
    _commands["NICK"] = &IrcServer::NICK;
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


int IrcServer::WaitEvent()
{
    int res_return;

    res_return = _MainServer->WaitEvent();
    return (res_return);
}

int IrcServer::CheckConnect()
{
    int res_return;

    res_return = _MainServer->CheckConnect();
    return (res_return);
}

int IrcServer::CheckDisconnect()
{
    int res_return;

    res_return = _MainServer->CheckDisconnect();
    return (res_return);
}

int IrcServer::CheckAndRead()
{
    int fd;

    fd = _MainServer->CheckAndRead();

    _MainServer->Logger(B_GRAY, "Read in " + std::to_string(fd) + " fd");
    return (fd);
}


void IrcServer::Start()
{
    int     events;
    int     connect;
    int     disconnect;
    int     reads_fd;
    
    events = this->WaitEvent();
    while (1)
    {
        if (events == 0)
            events = this->WaitEvent();

        connect = this->CheckConnect();
        if (connect > 0)
        {
            events--;
            continue;
        }
            

        disconnect = this->CheckDisconnect();
        if (disconnect > 0)
        {
            events--;
            continue;
        }
            

        reads_fd = this->CheckAndRead();
        if (reads_fd > 1)
        {
            this->ParseMessage(reads_fd);
            events--;
        }
        
    }
    
}



# endif