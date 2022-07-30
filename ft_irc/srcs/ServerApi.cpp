#include <ServerApi.hpp>


/* 
** Constructor
*/

void AbstractServerApi::Init(std::string& ipaddr, int port)
{
	_ipaddr = ipaddr;
	_port = port;
	
	std::cout << PURPLE;
	std::cout << "Ip address: " << _ipaddr << "\n";
	std::cout << "Port: " << _port << NORM << "\n";

	/*
		AF_UNIX, AF_LOCAL- Местная связь
		AF_INET- Интернет-протоколы IPv4
		AF_INET6- Интернет-протоколы IPv6
		AF_IPX- протоколы IPX Novell
	*/

	bzero(&_servaddr, sizeof(_servaddr));

	_servaddr.sin_family = SERVER_PROTOCOL;

	/* Port host to network short  */
	_servaddr.sin_port = htons(_port);
	/* преобразовать номер порта из порядка байтов хоста */
	_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/* Создаю сокет */
	Create_socket();
	/* Связываю его с адресом и портом*/
	Binded();
	/* Делаю сокет прослушивающим */
	Listen();

	#ifdef LOGGER_ENABLE
		std::string 	ports = std::to_string(_port);
		std::string		log_file_name = "";

		log_file_name += _ipaddr;
		log_file_name += "_";
		log_file_name += ports;
		log_file_name += "_log.txt";
		_logs.open(log_file_name, std::ios::ate);
		Logger(GREEN, std::string("Init Server!"));
	#endif
}

int AbstractServerApi::Create_socket()
{
	_server_fd = socket(_servaddr.sin_family, SERVER_TYPE, 0);

	if (_server_fd < 0)
		ServerError("Socket()");

	std::cout << GREEN << "Socket fd(" <<  _server_fd << ") successfully created ✅ " << NORM << "\n";
	
	/* Non block socket*/
    fcntl(_server_fd, F_SETFL, O_NONBLOCK);
	return (_server_fd);
}

int AbstractServerApi::Binded()
{
	int yes = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		ServerError("Setsokport");
	int bind_ = bind(_server_fd, (const struct sockaddr *)&_servaddr, sizeof(_servaddr));
	if (bind_ < 0)
		ServerError("Bind: ");
	std::cout << GREEN << "Success bind socket ✅ " << NORM << "\n";
	return bind_;
}

int AbstractServerApi::Listen()
{
	int _listen;

	_listen = listen(_server_fd, MAX_CONNECT_LISTEN);
	if (_listen < 0)
		ServerError("listen");
	std::cout << GREEN << "Server is listening " << MAX_CONNECT_LISTEN << " connections ✅ " << NORM << "\n";
	return (_listen);
}


/*
**	MAIN FUNCTIONAL
*/

int	AbstractServerApi::Accept()
{
	Logger(BLUE, "Accept...");

	struct sockaddr_in	clientaddr;
	socklen_t 			len;
	int 				client_fd;
	char 				buf[1024];

	// Incoming socket connection on the listening socket.
	// Create a new socket for the actual connection to client.
	client_fd = accept(_server_fd, (struct sockaddr *)&clientaddr, (socklen_t *)&len);
	if (client_fd == -1)
	{
		ServerError("Accept");
		return (-1);
	}
	AddClient(client_fd, clientaddr, _ipaddr);

	inet_ntop(SERVER_PROTOCOL, (char *)&(clientaddr.sin_addr), buf, sizeof(clientaddr));
	
	Logger(GREEN, "New connection as fd:(" + std::to_string(client_fd) + ") ✅ ");
	Logger(GREEN, "New connection as address: (" + std::string(buf) + ") ✅ ");
	Logger(GREEN, "New connection as port:(" + std::to_string(clientaddr.sin_port) + ") ✅ ");
	return (client_fd);
}

int AbstractServerApi::ReadInFd(int fd)
{
	char 		buffer[RECV_BUFFER_SIZE];

	bzero(buffer, RECV_BUFFER_SIZE);

	//printf("Read in fd:%d\n", fd);
	int ret = recv(fd, buffer, RECV_BUFFER_SIZE - 1, 0);
	if (ret == 0)
	{
		/* Api poll*/
		Logger(RED, "Disconnect fd(" + std::to_string(fd) + ") ❌ ");
		Logger(B_GRAY, "Remove fd " + std::to_string(fd));
		return (0);
	}
	else
	{
		_msg += buffer;

		//Logger(PURPLE, "Recv read " + std::to_string(ret) + " bytes");
		//Logger(B_GRAY, "buff:" + std::string(buffer));

		/* Read */
		while (ret == RECV_BUFFER_SIZE - 1)
		{
			ret = recv(fd, buffer, RECV_BUFFER_SIZE - 1, 0);
			if (ret == -1)
				break;

			buffer[ret] = 0;
			_msg += buffer;
			//Logger(B_GRAY, "subbuf:" + std::string(buffer));
			//Logger(PURPLE, "Replay Recv read " + std::to_string(ret));
		}

			Logger(GREEN, "Data is read is " + std::to_string(_msg.size()) + " bytes  ✅ ");
			Logger(B_GRAY, _msg);
			
			_msg = "";
			//USER Adium * 127.0.0.1 :Пользователь Adium
			//NICK ifanzilka
			
			//AbstractServerApi::SendInFd(fd, "Message Sucsefull\n", 18);
			//send(fd, "Message Sucsefull", 17, 0);
	}
	return (1);
}

int AbstractServerApi::SendInFd(int fd, std::string msg)
{
	char 	*c_msg = (char *)(const char *)msg.c_str();
	int 	res_send;
	
	size_t 	sended = 0;
	size_t 	len_msg = msg.size();
	size_t  BUFFER_LEN = BUFFER_SIZE_SEND > len_msg ? len_msg : BUFFER_SIZE_SEND;


	while (sended < len_msg)
	{
		res_send = send(fd, c_msg, BUFFER_LEN , 0);

		if (res_send == -1)
			ServerError("Send");

		sended += res_send;
		c_msg += res_send;

	}
	return (0);
}

int AbstractServerApi::SendInFd(int fd, const char *msg, size_t size)
{
	char 	*c_msg = (char *)(const char *)msg;
	int 	res_send;
	
	size_t 	sended = 0;
	size_t 	len_msg = size;
	size_t  BUFFER_LEN = BUFFER_SIZE_SEND > size ? size : BUFFER_SIZE_SEND;

	
	while (sended < len_msg)
	{
		std::cout << "send send start\n";
		res_send = send(fd, c_msg, BUFFER_LEN , 0);
		std::cout << "send send end\n";
		if (res_send == -1)
		{
			ServerError("Send");
			return (-1);
		}

		sended += res_send;
		c_msg += res_send;

	}
	return (0);
}


/* Clients */


/* Add */
void 	AbstractServerApi::AddClient(int fd_client, sockaddr_in addrinfo_client, std::string server_ipaddr)
{
	//Client   new_client(int fd_client, sockaddr_in addrinfo_client, std::string server_ipaddr);
	Client   *new_client = new Client (fd_client, addrinfo_client, server_ipaddr);


	_Clients.push_back(new_client);

}

/* Remove */
void 	AbstractServerApi::RemoveClient(int fd_client)
{
	std::vector<Client*>::iterator it = _Clients.begin();
	std::vector<Client*>::iterator it_end = _Clients.end();


	while (it < it_end)
	{
		if ((*it)->getFd() == fd_client)
		{
			delete *it;
			_Clients.erase(it);
			break;
		}

		it++;
	}
}



/*
** Getters
*/

std::string	AbstractServerApi::GetHostName() 
{
    return (_ipaddr);
}


int	AbstractServerApi::GetPort()
{
    return (_port);
}

/*
** FUNCTION
*/


void AbstractServerApi::Logger(std::string color, std::string msg)
{
	#ifdef LOGGER_ENABLE

	//char buffer[80];
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);

	_logs << "[";
	_logs << std::put_time(timeinfo, "%d") << " ";
	_logs << std::put_time(timeinfo, "%b") << " ";
	_logs << std::put_time(timeinfo, "%Y") << " ";
	_logs << std::put_time(timeinfo, "%I") << ":";
	_logs << std::put_time(timeinfo, "%M") << ":";
	_logs << std::put_time(timeinfo, "%S") << "]: ";
	_logs << msg << std::endl;

	std::cout << YELLOW;
	std::cout << "[";
	std::cout << std::put_time(timeinfo, "%d") << " ";
	std::cout << std::put_time(timeinfo, "%b") << " ";
	std::cout << std::put_time(timeinfo, "%Y") << " ";
	std::cout << std::put_time(timeinfo, "%I") << ":";
	std::cout << std::put_time(timeinfo, "%M") << ":";
	std::cout << std::put_time(timeinfo, "%S") << "]: " << NORM;
	std::cout << color << msg << NORM <<std::endl;
	
	#endif
}

void AbstractServerApi::PrintSockaddrInfo(struct sockaddr_in *info)
{
	char ip4[INET_ADDRSTRLEN]; // место для строки IPv4
	int port;
	std::string msg;


	port =  ntohs(info->sin_port);
	inet_ntop(AF_INET, &(info->sin_addr), ip4, INET_ADDRSTRLEN);//заполнили ip

	msg = "IPv4 address is: ";
	msg += std::string(ip4);
	msg += std::to_string(port);
	Logger(PURPLE, msg);
}

int	AbstractServerApi::SetNonBlockingFd(int fd)
{
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		ServerError("SetNonBlockingFd");
		return (-1);
	}
	return 0;
}

void AbstractServerApi::ServerError(const char *s)
{
	/* Смотрим ошибку из errno */
	char *str_error =  strerror(errno);
	std::string		err(str_error);
	std::string 	error_type(s);

	std::string 	full = "";

	/* Example: select: Bad decriptor */
	full += error_type;
	full += ": ";
	full += err;

	Logger(RED, full);
	std::cerr << RED << full << NORM << "\n";
	//throw std::runtime_error(full);
	//exit(42);
}


/*
** Destructor
*/

AbstractServerApi::~AbstractServerApi()
{
	Logger(RED, "Call ServerApi Destructor❌ ");
	close(_server_fd);
}