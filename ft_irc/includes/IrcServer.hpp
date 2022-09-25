#pragma once

#include "ServerApi.hpp"
#include <ServerPoll.hpp>
#include <ServerSelect.hpp>
#include <ServerKqueue.hpp>
#include <ServerEpoll.hpp>
#include "utility.hpp"

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


            std::cout << "res_return: " << fd << std::endl; 
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
            }
        }

        // void FindClientFromFd(int fd)
        // {
        //     _Clients

        // }

        //std::map<int , Client>;

        //мы спарсили пароль по нужному fd
        //теперь ищем клиента по fd

        void ParseMessage(int fd)
        {
            
            std::cout << "parse msg\n";
    

            std::vector<std::string> comands = ut::split(_MainServer->_msg, DELIMETER_COMAND); 
            std::vector<std::string> arguments;

            for (unsigned int i = 0; i < comands.size(); i++)
            {
                if (comands[i] != "")
                {
                    ut::ProcessingStr(comands[i]);
                    
                    std::vector<std::string> arguments = ut::splitForCmd(comands[i]);
                    fd--;
                    //MakeComand(arguments, fd);

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

        // void EndCicle()
        // {
        
        //     _MainServer->
        // }
        
            
        void	pass(std::vector<std::string>, int)
        {
            std::cout << "This is pass" << std::endl;
        
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

