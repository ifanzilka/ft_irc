#pragma once

#include "ServerApi.hpp"
#include <ServerPoll.hpp>
#include <ServerSelect.hpp>
#include <ServerKqueue.hpp>
#include <ServerEpoll.hpp>
#include "utility.hpp"
#include "IrcServerProtocol.hpp"

// #define SELECT  0
// #define POLL    1
// #define EPOLL   2
// #define KQUEUE  3

#define DELIMETER_COMAND "\n"

class IrcServer
{
    public:

        /* Constructors */
        IrcServer(int port, std::string password);

		IrcServer(std::string &ipaddr, int port, std::string password);

		IrcServer(const char *ipaddr, int port,std::string password);

        /* Destructor */
        ~IrcServer();

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
            int fd;

            fd = _MainServer->CheckAndRead();

            _MainServer->Logger(B_GRAY, "Read in " + std::to_string(fd) + " fd");
            return (fd);
        }

        bool CheckPassword(std::string &str)
        {
            if (_pass == str)
                return (true);
            return (false);
        }

        void ChangeClientStatus(int fd)
        {
            ClientIrc* clinet = _MainServer->GetClientFromFd(fd);

            if (clinet != NULL)
            {
                clinet->ChangeStatusConnect();
                _MainServer->Logger(GREEN, "Status client is changed");
            }
        }


        void ParseMessage(int fd)
        {
            _MainServer->Logger(B_GRAY, "Parse msg...");
    
            std::vector<std::string> comands = ut::split(_MainServer->_msg, DELIMETER_COMAND); 
            std::vector<std::string> arguments;

            for (unsigned int i = 0; i < comands.size(); i++)
            {
                if (comands[i] != "")
                {
                    ut::ProcessingStr(comands[i]);
                    
                    std::vector<std::string> arguments = ut::splitForCmd(comands[i]);
                    MakeComand(arguments, fd);

                }

            }
            _MainServer->_msg = "";
        }

        void    MakeComand(std::vector<std::string> &arguments, int fd)
        {
            if (_commands.find(arguments[0]) != _commands.end())
            {
                (this->*_commands[arguments[0]])(arguments, fd);
            }
        }

                
        void	pass(std::vector<std::string> arguments, int fd)
        {
            _MainServer->Logger(PURPLE, "Make command pass");

            if (arguments.size() > 1)
            {
                if (arguments[1] == _pass)
                {
                    _MainServer->Logger(GREEN, "Successfully password!");
                    ChangeClientStatus(fd);

                    //RPL_WELCOME("ifanzilka");
                    _MainServer->SendInFd(fd, RPL_WELCOME(std::string("ifanzilka")));
                    _MainServer->SendInFd(fd, RPL_MOTDSTART(std::string("ifanzilka")));
                    _MainServer->SendInFd(fd, RPL_MOTD(std::string("ifanzilka"), "hi"));
                }
            }
        
        }
        
    protected:
        /* Делаю сокращение */
        typedef	void (IrcServer::*commandPtr)(std::vector<std::string>, int);
        std::map<std::string, commandPtr>   _commands;


        AbstractServerApi *_MainServer; 
        std::string        _pass;
        std::string        _pass_hash;

    private:

        void InitComands();
        void InitIRC(std::string password, std::string ipaddr, int port);


};

