#include <IrcServer.hpp>


/*
** Регистрирует соединение клиента с сервером
**
** @Command: PASS
** @Parameters: <password>
**
*/

void	IrcServer::PASS(std::vector<std::string> arguments, int fd)
{
    ClientIrc *client;

    _MainServer->Logger(PURPLE, "Make command PASS");


    client = _MainServer->GetClientFromFd(fd);

    /* Если уже подключен */
    if (client->is_connected())
    {
        _MainServer->SendInFd(fd, ERR_ALREADYREGISTRED(client->getNickName()));
    }
    else if (arguments.size() > 1)
    {
       
        if (arguments[1] == _pass)
        {
            _MainServer->Logger(GREEN, "Successfully password!");
            ChangeClientStatus(fd);                   
        }
        else
        {
            _MainServer->SendInFd(fd, ERR_PASSWDMISMATCH(client->getNickName()));
        }
    }
    else
    {
        _MainServer->SendInFd(fd, ERR_NEEDMOREPARAMS(client->getNickName(), std::string("PASS")));
    }

}


/*
** Устанавливает юзернейм и реальное имя пользователя для регистрации
**
** @Command: USER
** @Parameters: <username> <host> <server> :<realname>
*/

void	IrcServer::USER(std::vector<std::string> arguments, int fd)
{
    ClientIrc *client;

    _MainServer->Logger(PURPLE, "Make command USER");

    client = _MainServer->GetClientFromFd(fd);

    if (!client->is_connected())
    {
        _MainServer->SendInFd(fd, ERR_NOTREGISTERED(client->getNickName()));
    }
    else if (arguments.size() == 5)
    {
        // std::cout << "1 " << arguments[0] << std::endl;  // USER
        // std::cout << "2 " << arguments[1] << std::endl;  // ifanzilka
        // std::cout << "3 " << arguments[2] << std::endl;  // *
        // std::cout << "4 " << arguments[3] << std::endl;  // 127.0.0.1
        // std::cout << "5 " << arguments[4] << std::endl;  // Fanzil

        /* Check name */
        for (std::vector<ClientIrc*>:: iterator start = _MainServer->_Clients.begin(); start != _MainServer->_Clients.end(); start++)
        {
            if ((*start)->getName() == arguments[1])
            {
                _MainServer->SendInFd(fd, ERR_ALREADYREGISTRED(client->getNickName()));
                return;
            }
        }
        client->setName(arguments[1]);
        client->SetRealName(arguments[4]);
        if (!client->is_authenticated())
        {
            client->ChangeStatusAuthenticated();
            WelcomeUser(client, fd);
        }
    }
    else
    {
        _MainServer->SendInFd(fd, ERR_NEEDMOREPARAMS(client->getNickName(), std::string("PASS")));
    }

}

void    IrcServer::WelcomeUser(ClientIrc *client, int fd)
{
    std::string nickname = client->getNickName();

    _MainServer->SendInFd(fd, RPL_MOTDSTART(nickname));
    _MainServer->SendInFd(fd, RPL_MOTD(nickname, "hi"));
    _MainServer->SendInFd(fd, RPL_ENDOFMOTD(nickname));
    _MainServer->SendInFd(fd, RPL_WELCOME(nickname));
    _MainServer->SendInFd(fd, RPL_YOUREOPER(nickname));         

}

/*
** Устанавливает новый ник или изменяет старый
**
** @Command: NICK
** @Parameters: <nickname>
**
*/

void	IrcServer::NICK(std::vector<std::string> arguments, int fd)
{
    ClientIrc *client;

    _MainServer->Logger(PURPLE, "Make command NICK");


    client = _MainServer->GetClientFromFd(fd);

    if (!client->is_connected())
    {
        _MainServer->SendInFd(fd, ERR_NOTREGISTERED(client->getNickName()));
    }

    else if (arguments.size() < 2)
    {
        _MainServer->SendInFd(fd, ERR_NONICKNAMEGIVEN(client->getNickName()));

    }
    else if (FindClientrByNickname(arguments[1]) != NULL)
    {
        _MainServer->SendInFd(fd, ERR_NICKNAMEINUSE(client->getNickName(), arguments[1]));   
    }/* Check valid Ncik*/
    else
    {
        if (!client->is_authenticated())
        {
            
            client->ChangeStatusAuthenticated();

            client->SetNickName(arguments[1]);
            WelcomeUser(client, fd);
        }
        else
        {
            client->SetNickName(arguments[1]);
            WelcomeUser(client, fd);
        }
    }
}

/*
** Сессия пользователя заканчивается с QUIT-сообщением.
**
** @Command: QUIT
** @Parameters: <message>
**
*/


void	IrcServer::QUIT(std::vector<std::string> arguments, int fd)
{
    ClientIrc *client;

    _MainServer->Logger(PURPLE, "Make command QUIT");

    client = _MainServer->GetClientFromFd(fd);
    _MainServer->SendInFd(fd, RPL_QUIT(client->getNickName(), arguments.size() > 1 ? arguments[1] : "silently"));

}


/*
** Проверка активности клиента/сервера на другой стороне соединения.
**
** @Command: PING
** @Parameters: <server>
**
*/

void	IrcServer::PING(std::vector<std::string> arguments, int fd)
{
    ClientIrc *client;

    _MainServer->Logger(PURPLE, "Make command PING");

    client = _MainServer->GetClientFromFd(fd);

    if (arguments.size() < 2)
    {
         _MainServer->SendInFd(fd, ERR_NOORIGIN(client->getNickName()));
    }
    else
    {
         _MainServer->SendInFd(fd, std::string("PONG ") + arguments[1] + std::string(" ") + client->getNickName());
    }

}


// void UsersService::ping(std::vector<std::string> args, int client_socket) {
//     if (args.size() < 2) {
//         _postman->sendReply(client_socket, ERR_NOORIGIN(_users[client_socket]->get_nickname()));

//     } else {
//         _postman->sendReply(client_socket, "PONG " + args[1] + " " + _users[client_socket]->get_nickname());
//     }
// }