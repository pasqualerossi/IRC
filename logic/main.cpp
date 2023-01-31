#include "headers/Server.hpp"
#include <signal.h>

bool	g_interrupt = false;

void	sigintHandler(int const sig) 
{
	(void)sig;
	g_interrupt = true;
	std::cout << "\b\b";
}

bool	getport(char *arg, int &port) 
{
	char	*buff;

	if (*arg == '\0')
		return (false);
	port = strtol(arg, &buff, 10);
	if (*buff != '\0')
	{
		std::cout << "Error input: port must contain only digits" << std::endl;
		return (false);
	}
	if (port < 0 || port > 65535)
	{
		std::cout << "Error input: port is out of range [0; 65535]" << std::endl;
		return (false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	int port;

	signal(SIGINT, sigintHandler);
	if (ac != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	if (getport(av[1], port) == false)
		return (1);
	
	try
	{
		Server	server(port, av[2]);
		server.launch();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}