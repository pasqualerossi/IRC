#include "irc.hpp"

bool	g_interrupt = false;

void	signalHandler(int const signal) 
{
	(void)signal;
	g_interrupt = true;
	std::cout << "\b\b";
}

bool	getport(char *argc, int &port) 
{
	char	*buffer;

	if (*argc == '\0')
		return (false);
	port = strtol(argc, &buffer, 10);
	if (*buffer != '\0')
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

int	main(int argc, char **argv)
{
	int port;

	signal(SIGINT, signalHandler);
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	if (getport(argv[1], port) == false)
		return (1);
	
	try
	{
		Server	server(port, argv[2]);
		server.launch();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}