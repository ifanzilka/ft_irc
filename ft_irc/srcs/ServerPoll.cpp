/* This code job in UNIX System */
#include "ServerPoll.hpp"

/* Constructors */
ServerPoll::ServerPoll(int port)
{
	std::string tmp = "127.0.0.1";
	AbstractServerApi::Init(tmp, port);
	Init_Serv();
}

ServerPoll::ServerPoll(const char *ipaddr, int port)
{
	std::string tmp = std::string(ipaddr);;
	AbstractServerApi::Init(tmp, port);
	Init_Serv();
}

ServerPoll::ServerPoll(std::string& ipaddr, int port)
{
	AbstractServerApi::Init(ipaddr, port);
	Init_Serv();
}

void ServerPoll::Init_Serv()
{
	_logs << "ServerType: Poll 🌐 " << std::endl;

	struct pollfd fd_serv;

	fd_serv.fd = _server_fd;
	fd_serv.events = POLLIN;	/* Какие события отслеживаю (входящие)*/
	fd_serv.revents = 0;

	_pollfds.push_back(fd_serv);
}


/* 
** Simple Use
*/

/* Help Function */
void 	ServerPoll::poll_add(int fd, short events)
{
	struct pollfd fd_client;

	fd_client.fd = fd;
	fd_client.events = events;	//говорю какие события слушаюй
	fd_client.revents = 0;
	_pollfds.push_back(fd_client);
}
void 	ServerPoll::poll_remove(int fd)
{
	(void)fd;
}


void ServerPoll::Start()
{
	Logger(BLUE, "Wait Event...");

	int result;
	int timeout = -1;

	/* Массив дискрипторов и размер его*/
	result = poll(& _pollfds[0], _pollfds.size(), timeout);
	Logger(B_GRAY, "Poll return " + std::to_string(result));
	
	if (result == -1)
	{
		ServerError("Poll: ");
	}
	else if (result == EINVAL)
	{
		Logger(RED, "TimeOut 🕐 ");
	}
	else
	{
		int 				client_fd;

		/* Check connect*/
		if( _pollfds[0].revents != 0)
		{
			client_fd = Accept();
			if (client_fd < 0)
			{
				if (errno != EWOULDBLOCK)
					ServerError("Accept");
			}
			else
			{// add fd
				AbstractServerApi::SetNonBlockingFd(client_fd);
				poll_add(client_fd, POLLIN);
			}
		}
		else
		{
			int fd_read;

			fd_read = 0;
			/* Read */
			Logger(BLUE, "Check read...");
			std::vector<struct pollfd>::iterator	it = _pollfds.begin();
			std::vector<struct pollfd>::iterator	it_end = _pollfds.end();

			while (it != it_end)
			{
				if (it->fd == _server_fd || it->revents  == 0)
				{
					it++;
					continue;
				}
				fd_read = it->fd;
				Logger(GREEN, "Readble is ready: fd(" + std::to_string(fd_read) + ") ✅ ");
				break;
			}

			if (fd_read != 0)
			{
				int res_read = AbstractServerApi::ReadInFd(fd_read);
				
				if (res_read == 0)
				{
					Logger(RED, "Disconnect fd(" + std::to_string(fd_read) + ") ❌ ");
					Logger(B_GRAY, "Remove fd " + std::to_string(fd_read));
					
					RemoveClient(it->fd);
					close(it->fd);
					_pollfds.erase(it);
				}
			}

		}
	}
}

/* Destructor */
ServerPoll::~ServerPoll()
{
	Logger(RED, "Call ServerPoll Destructor ❌ ");

	
}