/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:30:17 by prossi            #+#    #+#             */
/*   Updated: 2023/01/19 17:33:27 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// SERVER ACTIONS - display, add, erase, disconnect, read, handle, launch and find clients within a server.

Server::Server(int port, const String &password)
	: _host("127.0.0.1"), _password(password), _operator_password("operpass"), _port(port) 
	{
	_socket = create_socket();
}

Server::~Server() {}

int		Server::create_socket()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		throw std::runtime_error("Error while opening socket.");
	}
	int val = 1;
	if (setsockopt(sockfd, SOL_socketET, SO_REUSEADDR, &val, sizeof(val)))
	{
		throw std::runtime_error("Error while setting socket options.");
	}
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
	{
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");
	}
	sockaddr_in serv_address = {};
	bzero((char *) &serv_address, sizeof(serv_address));
	serv_address.sin_family = AF_INET;
	serv_address.sin_addr.s_addr = INADDR_ANY;
	serv_address.sin_port = htons(_port);

	if (bind(sockfd, (struct sockaddr *) &serv_address, sizeof(serv_address)) < 0)
		throw std::runtime_error("Error while binding socket.");
	if (listen(sockfd, 1000) < 0)
		throw std::runtime_error("Error while listening on socket.");
	return sockfd;
}

void	Server::display_the_Client()
{
	String state;
	size_t i = _clients.size();
	
	std::cout << "Clients connected: " << i << std::endl;

	for (size_t j = 0; j < i; j++)
	{
		state = (_clients.at(j).get_State() == REGISTERED) ? "yes" : "no";
		std::cout << "client[" << j << "]:" << " registered:" << state
		<< "   nick:" << _clients.at(j).get_Nick_name() 
		<< "   user:" <<_clients.at(j).get_User_name() 
		<< "   realname:" <<_clients.at(j).get_Real_Name() 
		<< std::endl;
	}
	std::cout << std::endl; 
	return ;
}

void	Server::adding_the_new_Client()
{
	int new_fd;
	char hostname[2048];
	
	sockaddr_in s_address = {};
	socklen_t s_size = sizeof(s_address);
	
	new_fd = accept(_socket, (sockaddr *) &s_address, &s_size);
	
	if (new_fd < 0)
	{
		throw std::runtime_error("Error while accepting new client.");
	}
	if (get_Nameinfo((struct sockaddr *) &s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) != 0)
	{
		throw std::runtime_error("Error while getting hostname on new client.");
	}
	_clients.push_back(Client(new_fd, hostname));
	pollfd pollfd = {new_fd, POLLIN, 0};
	_pollfds.push_back(pollfd);
}

void	Server::erase_the_Client(int fd)
{
	std::vector<Client>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		if (it->get_file_descriptor() == fd)
		{
			_clients.erase(it);
			return ;
		}
		it++;
	}
}

void	Server::client_Disconnect(int fd)
{
	std::vector<pollfd>::iterator it = _pollfds.begin();
	
	erase_Client_Channel(find_Client(fd));
	erase_the_Client(fd);
	
	while (it != _pollfds.end())
	{
		if (it->fd == fd)
		{
			_pollfds.erase(it);
			break;
		}
		it++;
	}
	close(fd);
	std::cout << "disconnexion succefull" << std::endl;
}

String	Server::read_Message(int fd) 
{
	String	msg;
	
	char	buff[256];
	bzero(buff, 256);
	
	std::vector<Client>::iterator cl = find_a_Client(fd);
	msg = cl->get_Message();
	
	while (!std::strstr(buff, "\n"))
	{
		int k = 0;
		bzero(buff, 256);
		if ((k = recv(fd, buff, 256, MSG_DONTWAIT)) < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				throw std::runtime_error("error in recv");
			}
			return ("");
		}
		else if (k == 0)
		{
			throw(std::out_of_range("TEST DECO"));
		}
		cl->addMsg(buff);
		msg += buff;
	}
	cl->set_Message("");
	return msg;
}

void	Server::handle_Message(int fd) 
{
	try 
	{
		this->_command = split_Command(read_Message(fd));
	}
	catch(const std::exception& e)
	{
		client_Disconnect(fd);
		std::cerr << e.what() << '\n';
		return ;
	}
	for (std::vector<String>::iterator it = this->_command.begin(); it != this->_command.end(); it++)
	{
		parse_Command(*it, find_Client(fd));
	}
	display_the_Client();
}

std::vector<String>	Server::split_Command(String msg) 
{
	std::vector<String> cmd;
	std::stringstream str(msg);
	
	String tmp;
	
	int i = 0;
	if (msg == "\n")
	{
		return cmd;
	}
	while (std::getline(str, tmp, '\n')) 
	{
		cmd.push_back(tmp);
		std::cout << cmd.at(i++) << std::endl;
	}
	return cmd;
}

void	Server::parse_Command(String str, Client &cl) 
{
	String tmp;

	std::vector<String>	args;
	std::stringstream ss(str);
	std::getline(ss, tmp, ' ');

	args.push_back(tmp);
  	
	std::cout << "Parse command : [" << tmp << "]" << std::endl;
	std::string cmds[15] = {"PASS", "NICK", "OPER", "USER", "PRIVMSG", "JOIN", "kill", "PING", "PART", "LIST", "NAMES", "TOPIC", "KICK", "MODE", "NOTICE"};

	int		(Server::*ptr[15])(std::vector<String> args, Client &cl) = 
	{
			&Server::password_Command,
			&Server::nickname_Command,
			&Server::operator_command,
			&Server::user_Command,
			&Server::private_message_Command,
			&Server::join_Command,
			&Server::kill_Command,
			&Server::ping_command,
			&Server::part_Command,
            &Server::command_List,
			&Server::channel_Names,
			&Server::topic_command,
			&Server::kick_Command,
			&Server::mode_Command,
			&Server::notice_Command,

	};
	for (int i =0; i <= 14; ++i)
	{
		if (tmp == cmds[i])
		{
			while (std::getline(ss, tmp, ' '))
				args.push_back(tmp);
			(this->*ptr[i])(args, cl);
			return;
		}
	}
}

void	Server::launch_Server()
{
	pollfd fd_server = {_socket, POLLIN, 0};
	_pollfds.push_back(fd_server);
	std::cout << BWHT << "Server IRC launch_Servered !" << RESET << std::endl;
	
	while (g_interrupt == false)
	{
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			break ;

		for (unsigned int i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents == 0)
			{
				continue ;
			}
			if ((_pollfds[i].revents  & POLLIN ) == POLLIN)
			{
				if (_pollfds[i].fd == _socket)
				{
					adding_the_new_Client();
					display_the_Client();
					break;
				}
			}
			handle_Message(_pollfds[i].fd);
		}
	}
	for (size_t i = 0; i < _pollfds.size(); i++)
	{
		close(_pollfds[i].fd);
	}
}

Client		&Server::find_Client(int fd)
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].get_file_descriptor() == fd)
			return (_clients[i]);
	}
	throw(std::out_of_range("Error while searching for user"));
}

Client		&Server::find_Client(String nick)
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].get_Nick_name() == nick)
		{
			return (_clients[i]);
		}
	}
	throw(std::out_of_range("Error while searching for user"));
}

std::vector<Client>::iterator	Server::find_a_Client(int fd)
{
	std::vector<Client>::iterator ret = _clients.begin();
	std::vector<Client>::iterator end = _clients.end();
	
	while (ret != end)
	{
		if (ret->get_file_descriptor() == fd)
		{
			return (ret);
		}
		ret++;
	}
	throw(std::out_of_range("Error while searching for user"));
}

std::vector<Channel>::iterator Server::find_a_Channel(std::string name)
{
	std::vector<Channel>::iterator  ret = _channels.begin();
	std::vector<Channel>::iterator  it_end = _channels.end();
	
	while (ret != it_end)
	{
		if (ret->get_Name() == name)
		{
			return (ret);
		}
		ret++;
	}
	throw (std::out_of_range("didnt find channel"));
}

Channel     &Server::find_Channel(std::string name)
{
	for (unsigned int i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].get_Name() == name)
		{
			return (_channels[i]);
		}
	}
	throw (std::out_of_range("didnt find channel"));
}

void    Server::erase_Client_Channel(Client &cl)
{
	for (unsigned int i = 0; i < _channels.size(); i++)
	{
		_channels[i].eraseClient(cl);
	}

	std::vector<Channel>::iterator   it = _channels.begin();
	while (it != _channels.end())
	{
		if (it->get_Clients().empty())
			it = _channels.erase(it);
		else
			it++;
	}
	std::cout << "eraseClientChannel" << std::endl;
}