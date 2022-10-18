#include "Channel.hpp"

/* Constructor */
//Channel::Channel(){}

Channel::Channel(std::string name, std::string pass, ClientIrc * main_client,IrcServer *ircservr)
{
    _IrcServer = ircservr;
    _channelName = name;
    _pass = pass;
    _clients.push_back(main_client);
    _opers.push_back(main_client);
    _topic = "Hello deat friend";
}

void Channel::AddClinet(ClientIrc *client)
{
    _clients.push_back(client);
}

void Channel::RemoveClient(ClientIrc *client)
{
    _clients.erase(std::find(_clients.begin(), _clients.end(), client));
}


void Channel::AddOper(ClientIrc *client)
{
    _opers.push_back(client);
}

void Channel::RemoveOper(ClientIrc *client)
{
    _opers.erase(std::find(_opers.begin(), _opers.end(), client));
}

bool    Channel::IsByClient(ClientIrc *client)
{
    if (std::find(_clients.begin(), _clients.end(), client) != _clients.end())
    {
        return true;
    } 
    return false;

}

bool    Channel::IsByOper(ClientIrc *client)
{
    if (std::find(_opers.begin(), _opers.end(), client) != _opers.end())
    {
        return true;
    } 
    return false;
}


void Channel::sendEveryone(std::string const &send, ClientIrc * sendUser)
{
    if(!IsByClient(sendUser) && sendUser != nullptr)
    {
        _IrcServer->SendInFd(sendUser->getFd(), ERR_NOTONCHANNEL(sendUser->getNickName(),this->_channelName));
    }
    
    
    for(std::vector<ClientIrc*>::iterator    iterClient = _clients.begin(); iterClient != _clients.end(); ++iterClient)
    {
        if(*iterClient != sendUser)
        {
            //придет define
            _IrcServer->SendInFd((*iterClient)->getFd(), send);
        }
    }

}

void    Channel::kickUser(ClientIrc *CallClient, ClientIrc *client, std::string &comment)
{
    if (!IsByOper(CallClient))
    {
        _IrcServer->SendInFd(CallClient->getFd(), ERR_CHANOPRIVSNEEDED(CallClient->getNickName(),this->_channelName));
    }
    else if (!IsByClient(client))
    {
        _IrcServer->SendInFd(CallClient->getFd(), ERR_NOSUCHNICK(CallClient->getNickName(),this->_channelName));
    }
    else
    {
        sendEveryone(RPL_KICK(CallClient->getNickName(), this->_channelName, client->getNickName(), comment), nullptr);
        RemoveClient(client);
    }
}


/* Destructor */
Channel::~Channel()
{
    //pass
}