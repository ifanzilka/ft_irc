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

        int     WaitEvent();
        int     CheckConnect();
        int     CheckDisconnect();
        int     CheckAndRead();
        void    Start();


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


        Client*       FindClientrByNickname(const std::string& nickname)
        {
            
            std::vector<ClientIrc*>::iterator iter_begin = _MainServer->_Clients.begin();
	        std::vector<ClientIrc*>::iterator iter_end = _MainServer->_Clients.end();

            while (iter_begin < iter_end)
            {
                
                if (nickname == (*iter_begin)->getNickName())
                {
                    return ((*iter_begin));
                }

                iter_begin++;
            }
            return (NULL);
        }

        void    WelcomeUser(ClientIrc *client, int fd);
        void	PASS(std::vector<std::string> arguments, int fd);
        void	USER(std::vector<std::string> arguments, int fd);
        void	NICK(std::vector<std::string> arguments, int fd);
        void	QUIT(std::vector<std::string> arguments, int fd);
        void	PING(std::vector<std::string> arguments, int fd);
        
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

