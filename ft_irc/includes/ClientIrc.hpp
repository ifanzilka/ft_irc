#ifndef FT_CLIENTIRC
# define FT_CLIENTIRC


class Client
{
	public:

		/* Constructor */
		Client(int fd_client, sockaddr_in 	addrinfo_client, std::string server_ipaddr)
		{
			_fd = fd_client;
			_addrinfo = addrinfo_client;
			_server_ipaddr = server_ipaddr;
		};

		int getFd() const
		{ 
			return (_fd);
		}
		
		struct sockaddr_in getAddrInfo() const
		{
			return (_addrinfo);
		}
	protected:
		struct sockaddr_in 	_addrinfo;
		int					_fd;
		std::string			_server_ipaddr;


};


class ClientIrc: public Client
{
    public:

        /* Constructor */
        ClientIrc(int fd_client, sockaddr_in 	addrinfo_client, std::string server_ipaddr): Client(fd_client, addrinfo_client, server_ipaddr)
        {
            std::string user("user_");

            _name = user + std::to_string(fd_client);
            _nickname = _name;
            _isconnect = false;
            _auntificated = false;
        };


        std::string getInfoClient()
        {
            std::string str;
            //char ip4[INET_ADDRSTRLEN]; // место для строки IPv4
            int port;

            port =  ntohs((&_addrinfo)->sin_port);
           // inet_ntop(AF_INET, &((&_addrinfo)->sin_addr), ip4, INET_ADDRSTRLEN);//заполнили ip

            str += "<-----Client Info----->\n";
            str += "Server IP Adress: " + _server_ipaddr + "\n";
            str += "Port: " + std::to_string(port) + "\n";
            str += "Name: " + getName() + "\n"; 
            return  (str);
        }

        std::string getName()
        {
            return (_name);
        } 
        
        std::string getNickName()
        {
            return (_nickname);
        } 
        
        void setName(std::string  name)
        {
            this->_name = name;
        }

        void SetNickName(std::string &name)
        {
            this->_nickname = name;
        }


        void ChangeStatusConnect()
        {
            _isconnect = true;
        }
        
        void ChangeStatusAuthenticated()
        {
            _auntificated = true;
        }

        bool is_connected()
        {
            return (_isconnect);
        }

        bool is_authenticated()
        {
            return (_auntificated);
        }




    private:
        std::string    _name;
        std::string    _nickname;
        bool           _isconnect; /* If succsecc password */
        bool           _auntificated;

};
 
#endif