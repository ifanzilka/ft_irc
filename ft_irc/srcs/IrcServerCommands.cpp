#include <IrcServer.hpp>

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


void	IrcServer::USER(std::vector<std::string> arguments, int fd)
{
    if (arguments[1] == "")
        return ;
    fd--;
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
        }

    }

}