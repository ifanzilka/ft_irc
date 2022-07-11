#include <iostream>
#include <ServerApi.hpp>
#include <ServerPoll.hpp>
#include <ServerSelect.hpp>

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;    
    
    if (argc > 1)
	{
		ServerSelect serv("127.0.0.1", atoi(argv[1]));
		//serv.Start();
	}
	else
	{
		std::cout << "Use: webserv port\n";
	}

    return (42);
}