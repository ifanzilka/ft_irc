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

#define DELIMETER_COMAND "\n"

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








            std::vector<std::string> res = split(_MainServer->_msg, DELIMETER_COMAND);

            for (unsigned int i = 0; i < res.size(); i++)
            {
                std::cout << "cmd: \n";
                std::cout << res[i] << std::endl;
                std::cout << "!" << res[i][0]  << "!" << std::endl;
                std::cout << "!" << res[i][1]  << "!" << std::endl;
                std::cout << "!" << res[i][2]  << "!" << std::endl;
                std::cout << "!" << res[i][3]  << "!" << std::endl;

                if (res[i].find("PASS") != std::string::npos)
                    std::cout << "Pass finded\n";
                //CheckCommand( res[i]);
            }
            _MainServer->_msg = "";
        }

        void CheckCommand(std::string &request)
        {
            
            //  if(request.find((char)5) != std::string::npos)
            //     request.erase(request.find((char)5));

        
            std::vector<std::string>    sumCommand;
            std::string                 temp = "";
            std::size_t                 pos;

            if(request.find('\n') != std::string::npos)
                request.erase(request.find('\n'));
            if(request.find('\r') != std::string::npos)
                request.erase(request.find('\r'));
            while(!isalpha(request[0]) || request.empty())
            {
                request.erase(0, 1);
            }

            if(request.empty())
            {
                sumCommand[0] = nullptr;
                // return sumCommand;
            }
            
            if(request.find(' ') != std::string::npos)
            {
                pos  = request.find(' ');
                temp = request.substr(0,pos);
                sumCommand.push_back(temp);
                request.erase(0, request.find(' ')+1);
            }
            if(request.find(" :") != std::string::npos)
            {
                pos = request.find(" :");
                std::cout << pos << "\n";
                temp = request.substr(0,pos);
                sumCommand.push_back(temp);
                temp = request.substr(pos+2);
                sumCommand.push_back(temp);
                request.erase(request.begin()+pos, request.end());
            }
            else
            {
                sumCommand.push_back(request);
            }
            
            
           
            // if(sumCommand[0].find("PASS") != std::string::npos)
            //     std::cout << "111COMPLETE111\n";
            //std::cout << "\\" + sumCommand[0] << '\n';
            //std::cout << "\\\\" + sumCommand[1] << '\n';


            std::cout << "comands:\n";

            if (sumCommand[0] == "USER")
            {
                std::cout << "pass complete\n";
            }
            std::cout << sumCommand[0] + "\n";
            std::cout << "len: " << sumCommand[0].size() << "\n";
            if (sumCommand.size() > 1)
                std::cout << sumCommand[1] + "\n";

            std::cout << "\n\n";

            // std::vector<std::string> res_split =  split(request, " ");

            // std::cout << "!" << res_split[0] << "!\n";
            // std::cout << "!" << res_split[1] << "!\n\n";

            // std::cout << "!" << res_split[0][0] << "!\n";
            // std::cout << "!" << res_split[0][1] << (int)res_split[0][1] <<"!\n";
            // std::cout << "!" << res_split[0][2] << "!\n";
            // std::cout << "!" << res_split[0][3] << "!\n";
            // std::cout << "!" << res_split[0][4] << "!\n";
            // std::cout << "!" << res_split[0].size() << "!\n";


            // // std::cout << "LEN: " << str.size() << std::endl;

            // if (res_split[0] == "PASS")
            // {
            //     std::cout << "Pass: " << split(request, " ")[1] << std::endl;
            // }

            // std::size_t found = request.find("PASS");
            // if (found != std::string::npos)
            // {
            //     std::cout << "Pass: " << split(request, ":")[1] << std::endl;
            // }


        }
        
        std::vector<std::string> split(std::string str, std::string const sep)
        {
            std::vector<std::string> result;
            if (str.find(sep) == std::string::npos)
                result.push_back(str);

            while (str.find(sep) != std::string::npos)
            {
                if (str.find(sep) == 0)
                {
                    str.erase(0);
                    continue;
                }
                else
                    result.push_back(str.substr(0, str.find(sep)));
                str.erase(0, str.find(sep) + 1);
                if ((str.find(sep) == std::string::npos))
                {
                    result.push_back(str);
                    break;
                }
            }
            return result;
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

