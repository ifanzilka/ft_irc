
#ifndef SERVER_EPOLL
#define SERVER_EPOLL

#ifdef __linux__

	#include "ServerApi.hpp"
	#include <sys/epoll.h>

	#define MAX_EVENTS 10
	#define EPOLL_SIZE 1024

	class ServerEpoll: public AbstractServerApi
	{
		public:

			/* Constructor */
			ServerEpoll(int port);
			ServerEpoll(std::string &ipaddr, int port);
			ServerEpoll(const char *ipaddr, int port);

			/* Simple use */
			void Start();

			/* Destructor */
			virtual ~ServerEpoll();

		private:

			/* Descriptor in  queue */
			struct epoll_event	_events[EPOLL_SIZE];
			int 				_epfd;
			
			/* Init Serv */
			void 	Init_Serv();

			/* Help Function */
			void 	epoll_add(int fd, uint32_t events);
	};

#endif
#endif