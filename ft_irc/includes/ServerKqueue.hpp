#ifndef SERVER_KQUEUE
# define SERVER_KQUEUE
#ifdef __APPLE__

/* Abstract Class  */
#include "ServerApi.hpp"
#include "Color.hpp"
#include <iostream>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>


#define 		KQUEUE_SIZE 64


class ServerKqueue: public AbstractServerApi
{
	public:

		/* Constructor */
		ServerKqueue(int port);
		ServerKqueue(std::string ipaddr, int port);
		ServerKqueue(const char *ipaddr, int port);
		
		/* Simple Use */
		void Start();

		//virtual int			WaitEvent(int &client_fd);
		//virtual int			CheckAccept();
		//virtual	int 		CheckRead();
		//virtual int 		CheckWrite();
		//virtual	int			ReadFd(int fd);

		/* Destructor */
		virtual ~ServerKqueue();

		//void	disableReadEvent(int socket, void *udata);
		//void	enableWriteEvent(int socket, void *udata);
		//void	disableWriteEvent(int socket, void *udata);
	private:
		/* Init */
		void 	Init_Serv();

		/* Help Function */
		void 	kqueue_add(int fd);
		void 	kqueue_remove(int fd);


		//void			addWriteEvent(int socket, void *udata);
		//void			addReadEvent(int socket, void *udata);
		
		int	 			_new_events;
		int				client_fd;

		/* Для макроса */
		struct kevent 	_evSet;
		struct kevent 	_evList[KQUEUE_SIZE];

		/* Очередь (дескриптор) */
		int			_kq_fd;

		
		//void 	AddFd(int fd);
		//void	RemoveFd(int client_fd);
};

#endif
#endif