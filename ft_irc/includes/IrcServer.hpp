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
            int res_return;

            res_return = _MainServer->CheckAndRead();
            std::cout << "res_return: " << res_return << std::endl; 
            return (res_return);
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

        void ParseMessage()
        {
            
            std::cout << "parse msg\n";
            //_MainServer->_msg = " ";



            // size_t pos = 0;
            // std::string token;
            // while ((pos = _MainServer->_msg.find(DELIMETER_COMAND)) != std::string::npos)
            // {
            //     token = _MainServer->_msg.substr(0, pos);
            //     //std::cout << token << std::endl;
            //     CheckCommand(token);
                
            //     _MainServer->_msg.erase(0, pos + 1);
            // }
            // std::cout << _MainServer->_msg << std::endl;

            std::vector<std::string> comands = ut::split(_MainServer->_msg, DELIMETER_COMAND); 
            std::vector<std::string> arguments;

            for (unsigned int i = 0; i < comands.size(); i++)
            {
                if (comands[i] != "")
                {

                    /* Очищаю от \r\n */
                    
                    if(comands[i].find('\n') != std::string::npos)
                        comands[i].erase(comands[i].find('\n'));
                    if(comands[i].find('\r') != std::string::npos)
                        comands[i].erase(comands[i].find('\r'));

                    std::cout << "!Check comand: !" << comands[i] << "!\n";
                    std::vector<std::string> arguments = ut::splitForCmd(comands[i]);
                    std::cout << arguments.size() << std::endl;
                    
                    for (unsigned int j = 0; j < arguments.size(); j++)
                    {
                        if (arguments[i] != "")
                        {
                            std::cout << "!cmds: !" << arguments[j] << "!\n";

                            //CheckCommand(res[i]);   
                        }
                    }

                    // if (_commands.find(arguments[0]) != _commands.end())
                    // {
                    //     (this->*_commands[arguments[0]])(arguments, client_socket);
                    // }

                    //CheckCommand(res[i]);
                    
                }

            }
            _MainServer->_msg = "";
        }

        void CheckCommand(std::string &request)
        {
            std::vector<std::string> split_cmd;

            if(request.find('\n') != std::string::npos)
                request.erase(request.find('\n'));
            if(request.find('\r') != std::string::npos)
                request.erase(request.find('\r'));

            while (request.find(' ') != std::string::npos)
                request.erase(request.find(' '));

            

            split_cmd = ut::split(request, " ");

            for (unsigned int i = 0; i < split_cmd.size(); i++)
            {
                if (split_cmd[i] != "")
                {
                    std::cout << "!cmd_split !" << split_cmd[i] << "!\n";
                }
            }
        }

        
    void	pass(std::vector<std::string>, int)
    {


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

