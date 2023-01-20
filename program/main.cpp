/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:21:23 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 13:39:53 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

bool	g_interrupt = false;

void	signal_Handler(int const signal) 
{
	(void)signal;
	g_interrupt = true;
	std::cout << "\b\b";
}

bool	get_port(char *argc, int &port) 
{
	char	*buffer;
	
	if (*argc == '\0')
	{
		return (false);
	}
	port = strtol(argc, &buffer, 10);
	if (*buffer != '\0')
	{
		std::cout << RED << "Error input: port must contain only digits" << RESET << std::endl;
		return (false);
	}
	if (port < 0 || port > 65535)
	{
		std::cout << RED << "Error input: port is out of range [0; 65535]" << RESET << std::endl;
		return (false);
	}
	return (true);
}

int	main(int argc, char **argv)
{
	int port;

	signal(SIGINT, signal_Handler);
	if (argc != 3)
	{
		std::cout << BWHT << "Usage: ./ircserv <port> <password>" << RESET << std::endl;
		return (1);
	}
	if (get_port(argv[1], port) == false)
	{
		return (1);
	}
	try
	{
		Server	server(port, argv[2]);
		server.launch_Server();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}