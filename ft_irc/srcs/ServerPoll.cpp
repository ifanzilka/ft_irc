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
	fd_serv.events = POLLIN;
	fd_serv.revents = 0;

	_pollfds.push_back(fd_serv);
}


/* 
** Simple Use
*/

void ServerPoll::Start()
{
	Logger(BLUE, "Wait Event...");

	int result;
	int timeout = -1;

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

				struct pollfd fd_client;

				fd_client.fd = client_fd;
				fd_client.events = POLLIN;	//говорю какие события слушаюй
				fd_client.revents = 0;
				_pollfds.push_back(fd_client);

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
				char buffer[RECV_BUFFER_SIZE];
				bzero(buffer, RECV_BUFFER_SIZE);

				int ret = recv(fd_read, buffer, RECV_BUFFER_SIZE - 1, 0);
				if (ret == 0)
				{
					Logger(RED, "Disconnect fd(" + std::to_string(fd_read) + ") ❌ ");
					Logger(B_GRAY, "Remove fd " + std::to_string(fd_read));
					close(it->fd);
					_pollfds.erase(it);

				}
				else
				{
					//_client_rqst_msg.resize(0);
					//client_rqst_msg += buffer;
					std::string msg;

					msg += buffer;

					Logger(PURPLE, "Recv read " + std::to_string(ret) + " bytes");
					Logger(B_GRAY, "buff:" + std::string(buffer));

					while (ret == RECV_BUFFER_SIZE - 1)
					{
						ret = recv(client_fd, buffer, RECV_BUFFER_SIZE - 1, 0);
						if (ret == -1)
							break;

						buffer[ret] = 0;
						msg += buffer;
						Logger(B_GRAY, "subbuf:" + std::string(buffer));
						Logger(PURPLE, "Replay Recv read " + std::to_string(ret) + " bytes");
					}

						Logger(GREEN, "Data is read is " + std::to_string(msg.size()) + " bytes  ✅ ");
						Logger(B_GRAY, msg);

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

	// /* Start */
	// int ServerPoll::WaitEvent(int &client_fd)
	// {
	// 	int result;

	// 	Logger(BLUE, "Wait Event...");
	// 	result = poll(& _pollfds[0], _pollfds.size(), -1);
	// 	Logger(B_GRAY, "Poll return " + std::to_string(result));

	// 	if (result == -1)
	// 	{
	// 		ServerError("Poll");
	// 	}
	// 	else if (result == 0)
	// 	{
	// 		Logger(RED, "TimeOut 🕐 ");
	// 	}
	// 	return result;
	// }

	// void ServerPoll::AddFd(int fd)
	// {
	// 	Logger(B_GRAY, "Add fd " + std::to_string(fd));
	// 	struct pollfd fd_client;

	// 	fd_client.fd = fd;
	// 	fd_client.events = POLLIN;
	// 	fd_client.revents = 0;
	// 	_pollfds.push_back(fd_client);
	// }

	// /* Проверяю событие на Подключение если полюкчился возвращаю fd клиента иначе 0*/
	// int ServerPoll::CheckAccept()
	// {
	// 	Logger(BLUE, "Check Accept...");
	// 	int 				client_fd;

	// 	if( _pollfds[0].revents == 0)
	// 		return (0);

	// 	client_fd = Accept();
	// 	if (client_fd < 0)
	// 	{
	// 		if (errno != EWOULDBLOCK)
	// 			ServerError("Accept");
	// 		return -1;
	// 	}
	// 	AddFd(client_fd);
	// 	return (client_fd);
	// }

	// /* Проверяю на чтение если нету возвращаю 0, иначе fd откуда читать */
	// int ServerPoll::CheckRead()
	// {
	// 	Logger(BLUE, "Check read...");
	// 	std::vector<struct pollfd>::iterator	it = _pollfds.begin();
	// 	std::vector<struct pollfd>::iterator	it_end = _pollfds.end();

	// 	while (it != it_end)
	// 	{
	// 		if (it->fd == _server_fd || it->revents  == 0)
	// 		{
	// 			it++;
	// 			continue;
	// 		}

	// 		return(it->fd);
	// 		it++;
	// 	}
	// 	return (0);
	// }

	// //TODO: вынести этот метод в абстрактный класс, ибо реализация везде одинаковая
	// int ServerPoll::ReadFd(int client_fd)
	// {
	// 	Logger(GREEN, "Readble is ready: fd(" + std::to_string(client_fd) + ") ✅ ");

	// 	char buffer[RECV_BUFFER_SIZE];
	// 	bzero(buffer, RECV_BUFFER_SIZE);

	// 	int ret = recv(client_fd, buffer, RECV_BUFFER_SIZE - 1, 0);
	// 	if (ret == 0)
	// 	{
	// 		Logger(RED, "Disconnect fd(" + std::to_string(client_fd) + ") ❌ ");
	// 		RemoveFd(client_fd);
	// 		return (0);
	// 	}

	// 	_client_rqst_msg.resize(0);
	// 	_client_rqst_msg += buffer;

	// 	Logger(PURPLE, "Recv read " + std::to_string(ret) + " bytes");
	// 	Logger(B_GRAY, "buff:" + _client_rqst_msg);
	// 	while (ret == RECV_BUFFER_SIZE - 1)
	// 	{
	// 		ret = recv(client_fd, buffer, RECV_BUFFER_SIZE - 1, 0);
	// 		if (ret == -1)
	// 			break;

	// 		buffer[ret] = 0;
	// 		_client_rqst_msg += buffer;
	// 		Logger(B_GRAY, "subbuf:" + std::string(buffer));
	// 		Logger(PURPLE, "Replay Recv read " + std::to_string(ret) + " bytes");
	// 	}

	// 	Logger(GREEN, "Data is read is " + std::to_string(_client_rqst_msg.size()) + " bytes  ✅ ");
	// 	Logger(B_GRAY, _client_rqst_msg);

	// 	return (_client_rqst_msg.size());
	// }

	// void ServerPoll::RemoveFd(int client_fd)
	// {
	// 	Logger(B_GRAY, "Remove fd " + std::to_string(client_fd));

	// 	std::vector<struct pollfd>::iterator it = _pollfds.begin();
	// 	std::vector<struct pollfd>::iterator it_end = _pollfds.end();

	// 	while (it != it_end)
	// 	{
	// 		if (it->fd == client_fd)
	// 		{
	// 			close(it->fd);
	// 			_pollfds.erase(it);
	// 			return;
	// 		}
	// 		it++;
	// 	}
	// 	RemoveClient(client_fd);
	// }

	// /* Destructor */
	// ServerPoll::~ServerPoll()
	// {
	// 	Logger(RED, "Call ServerPoll Destructor ❌ ");
	// }