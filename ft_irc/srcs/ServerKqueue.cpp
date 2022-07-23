/* This code job in BSD System */

#ifdef __APPLE__
	
	#include "ServerKqueue.hpp"

	/* Constructors */
	ServerKqueue::ServerKqueue(int port)
	{
		std::string tmp = "127.0.0.1";
		AbstractServerApi::Init(tmp, port);
		Init_Serv();
	}

	ServerKqueue::ServerKqueue(const char *ipaddr, int port)
	{
		std::string tmp = std::string(ipaddr);;
		AbstractServerApi::Init(tmp, port);
		Init_Serv();
	}

	ServerKqueue::ServerKqueue(std::string ipaddr, int port)
	{
		AbstractServerApi::Init(ipaddr, port);
		Init_Serv();
	}

	void 	ServerKqueue::Init_Serv()
	{
		_logs << "ServerType: Kqueue 🌐 " << std::endl;

		int res_kevent;

		/* 
		** Системный вызов kqueue () создает новую очередь событий ядра и возвращает 
     	** дескриптор 
		*/

		_kq_fd = kqueue();
		if (_kq_fd == -1)
			ServerError("Kqueue");

		/* Добавляю событие прослушки fd сервера */
		/* Задаем события для отслеживания */
		/* EV_SET() — это макрос, предназначенный для упрощения инициализации структуры kevent . */
		/* EVFILT_READ - cобытия , EV_ADD | EV_ENABLE - фильтры его */
		/* Грубо говоря настриваю сокет сервера на прослушивание */
		EV_SET(&_evSet, _server_fd, EVFILT_READ, EV_ADD | EV_ENABLE , 0, 0, NULL);


		/* 
		**  Системный вызов kevent () используется для регистрации событий в очереди и 
		** возврата пользователю любых ожидающих событий. Аргумент списка изменений — это указатель 
		** на массив структур kevent
		*/
		
		res_kevent = kevent(_kq_fd, &_evSet, 1, NULL, 0, NULL);
		if (res_kevent == -1)
			ServerError("Kevent");

		bzero(&_evSet, sizeof(_evSet));
	}



	/*
	**	Simple Use
	*/


	void 	ServerKqueue::kqueue_add(int fd)
	{
		struct kevent kv;
		
		/* EV_ADD -> добавляю в очередь на отслеживание */
		EV_SET(&kv, fd, EVFILT_READ, EV_ADD, 0 , 0, NULL);
		if (kevent(_kq_fd, &kv, 1, NULL, 0, NULL) == -1)
			ServerError("kevent add");

	}

	void 	ServerKqueue::kqueue_remove(int fd)
	{
		struct kevent kv;
		
		/* EV_DISABLE -> удаляю дескриптор из очереди отслеживания */
		EV_SET(&kv, fd, EVFILT_READ, EV_DISABLE, 0 , 0, NULL);
		if (kevent(_kq_fd, &kv, 1, NULL, 0, NULL) == -1)
			ServerError("kevent remove");
		
	}


	void 	ServerKqueue::Start()
	{
		Logger(BLUE, "Wait kevent...");
		bzero(_evList, sizeof(_evList));
		
		struct timespec ts;
		ts.tv_sec = 0;
		ts.tv_nsec = 0;

		_new_events = kevent(_kq_fd, NULL, 0, _evList, KQUEUE_SIZE, NULL);
		if (_new_events == -1)
			ServerError("kevent");


		for (int i = 0; i < _new_events; i++)
		{
			client_fd = _evList[i].ident;

			// When the client disconnects an EOF is sent. By closing the file
			// descriptor the event is automatically removed from the kqueue.
			if (_evList[i].flags & EV_EOF)
			{
				Logger(RED, "Disconnect fd(" + std::to_string(client_fd) + ") ❌ ");				
				kqueue_remove(client_fd);
				RemoveClient(client_fd);
			}
			else if (client_fd == _server_fd)
			{
				int new_client_fd;
				new_client_fd = Accept();
				Logger(GREEN, "Connect fd(" + std::to_string(new_client_fd) + ") ✅ ");
				kqueue_add(new_client_fd);
			}
			else if (_evList[i].filter == EVFILT_READ)
			{
				AbstractServerApi::ReadInFd(client_fd);
				//event_flag = EVFILT_READ;
			}
			else if (_evList[i].filter == EVFILT_WRITE)
			{
				//event_flag = EVFILT_WRITE;
			}
		}

	}





	// int	ServerKqueue::WaitEvent(int &client_fd)
	// {
	// 	bzero(_evList, sizeof(_evList));
	// 	struct timespec ts;
	// 	ts.tv_sec = 3;
	// 	ts.tv_nsec = 0;

	// 	//
	// 	_new_events = kevent(_kq_fd, NULL, 0, _evList, KQUEUE_SIZE, &ts);
	// 	if (_new_events == -1)
	// 		ServerError("kevent");

	// 	//Logger(B_GRAY, "kevent return " + std::to_string(_new_events));

	// 	int event_flag = 0;

	// 	for (int i = 0; i < _new_events; i++)
	// 	{
	// 		client_fd = _evList[i].ident;

	// 		// When the client disconnects an EOF is sent. By closing the file
	// 		// descriptor the event is automatically removed from the kqueue.
	// 		if (_evList[i].flags & EV_EOF)
	// 		{
	// 			//TODO: std::to_string - 11CPP
	// 			Logger(RED, "Disconnect fd(" + std::to_string(client_fd) + ") ❌ ");
	// 			RemoveFd(client_fd);
	// 			event_flag = EV_EOF;
	// 		}
	// 		else if (client_fd == _server_fd)
	// 		{
	// 			client_fd = CheckAccept();
	// 			Logger(GREEN, "Connect fd(" + std::to_string(client_fd) + ") ✅ ");
	// 		}
	// 		else if (_evList[i].filter == EVFILT_READ)
	// 		{
	// 			event_flag = EVFILT_READ;
	// 		}
	// 		else if (_evList[i].filter == EVFILT_WRITE)
	// 		{
	// 			event_flag = EVFILT_WRITE;
	// 		}
	// 	}

	// 	return (event_flag);
	// }

	// //TODO: CheckAccept возвращает фд клиента, но не используется нигде

	// /* Проверяю событие на Подключение если полюкчился возвращаю fd клиента иначе 0*/
	// int ServerKqueue::CheckAccept()
	// {
	// 	//Logger(BLUE, "CheckAccept...");
	// 	int client_fd;

	// 	for (int i = 0; i < _new_events; i++)
	// 	{
	// 		int event_fd = _evList[i].ident;

	// 		if (event_fd == _server_fd)
	// 		{
	// 			break;
	// 		}
	// 		return (0);
	// 	}

	// 	client_fd = Accept();
	// 	if (client_fd == -1)
	// 	{
	// 		return -1;
	// 	}
	// 	AddFd(client_fd);
	// 	return (client_fd);
	// }

	// /* Проверяю на чтение если нету возвращаю 0, иначе fd откуда читать */
	// int ServerKqueue::CheckRead()
	// {
	// 	Logger(BLUE, "Check Read ...");

	// 	for (int i = 0; i < _new_events; i++)
	// 	{
	// 		int event_fd = _evList[i].ident;

	// 		// When the client disconnects an EOF is sent. By closing the file
	// 		// descriptor the event is automatically removed from the kqueue.
	// 		if (_evList[i].flags & EV_EOF)
	// 		{
	// 			Logger(RED, "Disconnect fd(" + std::to_string(event_fd) + ") ❌ ");
	// 			RemoveFd(event_fd);
	// 		}
	// 		else if (_evList[i].filter & EVFILT_READ)
	// 		{
	// 			return (event_fd);
	// 		}
	// 	}
	// 	return (0);
	// }

	// void ServerKqueue::disableReadEvent(int socket, void *udata)
	// {
	// 	struct kevent kv;
	// 	EV_SET(&kv, socket, EVFILT_READ, EV_DISABLE, 0 , 0, udata);
	// 	if (kevent(_kq_fd, &kv, 1, NULL, 0, NULL) == -1)
	// 		std::cerr << "kevent() disableReadEvent error" << std::endl;

	// 	std::cout << B_CYAN"READ EVENT DISABLED"NORM << std::endl; //TODO: удалить
	// }

	// void ServerKqueue::enableWriteEvent(int socket, void *udata)
	// {
	// 	struct kevent kv;
	// 	EV_SET(&kv, socket, EVFILT_WRITE, EV_ENABLE, 0 , 0, udata);
	// 	if (kevent(_kq_fd, &kv, 1, NULL, 0, NULL) == -1)
	// 		std::cerr << "kevent() enableWriteEvent error" << std::endl;

	// 	std::cout << B_GREEN"WRITE EVENT ENABLED"NORM << std::endl; //TODO: удалить
	// }

	// void ServerKqueue::disableWriteEvent(int socket, void *udata)
	// {
	// 	struct kevent kv;
	// 	EV_SET(&kv, socket, EVFILT_WRITE, EV_DISABLE, 0 , 0, udata);
	// 	if (kevent(_kq_fd, &kv, 1, NULL, 0, NULL) == -1)
	// 		std::cerr << "kevent() disableWriteEvent error" << std::endl;

	// 	std::cout << B_CYAN"WRITE EVENT DISABLED"NORM << std::endl; //TODO: удалить
	// }

	// void ServerKqueue::addReadEvent(int socket, void *udata)
	// {
	// 	struct kevent kv;
	// 	EV_SET(&kv, socket, EVFILT_READ, EV_ADD, 0 , 0, udata);
	// 	if (kevent(_kq_fd, &kv, 1, NULL, 0, NULL) == -1)
	// 		std::cerr << "kevent() addReadEvent error" << std::endl;

	// 	std::cout << B_CYAN"READ EVENT ADDED"NORM << std::endl; //TODO: удалить
	// }

	// void ServerKqueue::addWriteEvent(int socket, void *udata)
	// {
	// 	struct kevent kv;
	// 	EV_SET(&kv, socket, EVFILT_WRITE, EV_ADD , 0 , 0, udata);
	// 	if (kevent(_kq_fd, &kv, 1, NULL, 0, NULL) == -1)
	// 		std::cerr << "kevent() addWriteEvent error" << std::endl;

	// 	std::cout << B_CYAN"WRITE EVENT ADDED"NORM << std::endl; //TODO: удалить
	// }

	// /**
	//  * Добавляет отслеживаемые события в kqueue:
	//  * 1) чтение
	//  * 2) запись
	//  * 3) временно отключает сигнал о готовности записи
	//  * */
	// void ServerKqueue::AddFd(int fd)
	// {
	// 	std::cout << BLUE"STARTING A NEW CONNECTION..."NORM << std::endl; //TODO: удалить
	// 	Logger(B_GRAY, "Add fd " + std::to_string(fd));

	// 	addReadEvent(fd, this);
	// 	addWriteEvent(fd, this);
	// 	disableWriteEvent(fd, this);
	// }

	// void ServerKqueue::RemoveFd(int client_fd)
	// {
	// 	Logger(B_GRAY, "Remove fd " + std::to_string(client_fd));

	// 	close(client_fd);
	// 	RemoveClient(client_fd);
	// }

	ServerKqueue::~ServerKqueue()
	{
		close(_kq_fd);
		Logger(RED, "Call ServerKqueue Destructor ❌ ");
	}

#endif
