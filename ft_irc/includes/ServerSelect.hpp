#ifndef SERVER_SELECT
# define SERVER_SELECT

/* Abstract Class  */
#include "ServerApi.hpp"
#include "Color.hpp"

#include <iostream> /* string */
#include <vector>	/* vector */
#include <map>		/* map	*/
#include <unistd.h> /* write */

class ServerSelect: public AbstractServerApi
{
	public:

		/* Constructors */
		ServerSelect(int port);
		ServerSelect(std::string &ipaddr, int port);
		ServerSelect(const char *ipaddr, int port);
		
		/* Simple USE */
		void Start();


		/* Main Functional*/
		int		WaitEvent();
		int		CheckConnect();
		int		CheckDisconnect();
		int		CheckAndRead();

		/* USE */
		//void Run();

		/* Events */
		//virtual	int			WaitEvent(int &client_fd);
		//virtual	int			CheckAccept();
		//virtual	int			CheckRead();
		//virtual void 		CheckWrite();
		//virtual	int			ReadFd(int clinet_fd);


		/* Main functional */
		//void 		Run();

		/* Destructor */
		virtual ~ServerSelect();

	private:
		int					_max_fd;

		/* For select */
		fd_set				_currfds;
		fd_set 				_writefds;
		fd_set 				_readfds;

		void 	Init_Serv();

		/* Help Function */
		void 	select_add(int fd);
		void 	select_remove(int fd);
		int		check_disconnect(int fd);
};

#endif