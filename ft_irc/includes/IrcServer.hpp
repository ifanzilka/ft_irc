#pragma once

#include "ServerApi.hpp"
#include <ServerPoll.hpp>
#include <ServerSelect.hpp>
#include <ServerKqueue.hpp>
#include <ServerEpoll.hpp>


// #define SELECT  0
// #define POLL    1
// #define EPOLL   2
// #define KQUEUE  3



class IrcServer
{
    public:
        

        /* Constructors */
        IrcServer(int port, std::string password)
        {
            std::string tmp = "127.0.0.1";
            
            IrcServer::InitIRC(password, tmp, port);
        }

		IrcServer(std::string &ipaddr, int port, std::string password)
        {
            IrcServer::InitIRC(password, ipaddr, port);
        }

		IrcServer(const char *ipaddr, int port,std::string password)
        {
            std::string tmp = std::string(ipaddr);;

            IrcServer::InitIRC(password, tmp, port);
        }

        /* Destructor */
        ~IrcServer()
        {
            delete _MainServer;
        }

        /* Function Main Server */

        int WaitEvent()
        {
            int res_return;

            res_return = _MainServer->WaitEvent();
            return (res_return);
        }

        int CheckConnect()
        {
            int res_return;

            res_return = _MainServer->CheckConnect();
            return (res_return);
        }

        int CheckDisconnect()
        {
            int res_return;

            res_return = _MainServer->CheckDisconnect();
            return (res_return);
        }

        int CheckAndRead()
        {
            int res_return;

            res_return = _MainServer->CheckAndRead();
            return (res_return);
        }
        

    protected:
        AbstractServerApi *_MainServer;
        std::string        _pass;
        std::string        _pass_hash;

    private:

        void InitIRC(std::string password, std::string ipaddr, int port)
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

            
        
            _pass = password;
        }


};

