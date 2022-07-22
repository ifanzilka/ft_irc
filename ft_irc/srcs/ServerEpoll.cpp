/* This code job in linux System */
#ifdef __linux__
#include "ServerEpoll.hpp"

/* Constructors */
void ServerEpoll::Init_Serv()
{
	_logs << "ServerType: Epoll 🌐 " << std::endl;
	int res;

	struct epoll_event event;
	event.data.fd = _server_fd;
  	event.events = EPOLLIN | EPOLLET; /* Отслеживаю чтение и */

	/* Создаем очеред */
	_epfd = epoll_create(MAX_EVENTS);
	
	if (_epfd == -1)
		ServerError("Epoll_create");
	
	/*  Добавляю fd в очередь epfd чтобы мог отслеживать его события*/
	res = epoll_ctl(_epfd, EPOLL_CTL_ADD, _server_fd, &event);
	if (res == -1)
		ServerError("Epoll Init epoll_ctl");
	

}

ServerEpoll::ServerEpoll(int port)
{
	std::string tmp = "127.0.0.1";
	AbstractServerApi::Init(tmp, port);
	Init_Serv();
}

ServerEpoll::ServerEpoll(std::string& ipaddr, int port)
{
	AbstractServerApi::Init(ipaddr, port);
	Init_Serv();
}

ServerEpoll::ServerEpoll(const char *ipaddr, int port)
{
	std::string tmp = std::string(ipaddr);;
	AbstractServerApi::Init(tmp, port);
	Init_Serv();
}


/* Simple Use */

void ServerEpoll::Start()
{
	Logger(BLUE, "Wait epoll_wait...");
	
	int n_events;
	int timeout;

	timeout = -1;
	n_events = epoll_wait(_epfd, _events, EPOLL_SIZE, timeout);

	for (int i = 0; i < n_events; i++)
	{
		if (_events[i].data.fd == _server_fd)
		{

			int client_fd = Accept();
			if (client_fd < 0)
			{
				ServerError("Accept");
			}
			else
			{
				struct epoll_event ev;
				ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP;
				ev.data.fd = client_fd;
				if (epoll_ctl(_epfd, EPOLL_CTL_ADD, client_fd, &ev) == -1)
				{
					ServerError("Epoll epoll_ctl add new fd");
				}

			}
			
		} 
		else if (_events[i].events & EPOLLIN)
		{
			/* handle EPOLLIN event */
			//printf("POLLIN");
			int rc;
			char buffer[1024];

			bzero(buffer, 1024);
			size_t bytes_rea = recv(_events[i].data.fd, buffer, sizeof(buffer), 0);
			printf("read %zu bytes\n", bytes_rea);
			Logger(GREEN, std::to_string(_events[i].data.fd) + " message:\n" + std::string(buffer));
			send(_events[i].data.fd, "Message Sucsefull", 17, 0);

		} 
		else
		{
			printf("[+] unexpected\n");
		}

		if (_events[i].events & (EPOLLRDHUP | EPOLLHUP))
		{
				Logger(RED, std::to_string(_events[i].data.fd) + " Connection close ❌");
				epoll_ctl(_epfd, EPOLL_CTL_DEL, _events[i].data.fd, NULL);
				close(_events[i].data.fd);
				continue;
		}
	}
}

/* Destructor */
ServerEpoll::~ServerEpoll()
{
	close(_epfd);
	Logger(RED, "Call ServerEpoll Destructor ❌ ");
}



#endif