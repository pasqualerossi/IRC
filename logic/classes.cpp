/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   classes.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/02 14:23:44 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/*

1st Class - Server
2nd Class - Channel
3rd Class - Client

*/

/* ***************************** SERVER CLASS ******************************* */

Server::Server(int port, const String &pass)
	: _host("127.0.0.1"), _password(pass), _operPassword("operator's password"), _port(port) 
{
	_sock = createSocket();
}

Server::~Server() {}

int		Server::createSocket()
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Error while opening socket.");

	int val = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)))
		throw std::runtime_error("Error while setting socket options.");

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1) 
		throw std::runtime_error("Error while setting socket to NON-BLOCKING.");

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

void	Server::displayClient()
{
	String state;
	size_t i = _clients.size();
	std::cout << "Clients connected: " << i << std::endl;
	for (size_t j = 0; j < i; j++)
	{
		state = (_clients.at(j).getState() == REGISTERED) ? "yes" : "no";
		std::cout << "client[" << j << "]:" << " registered:" << state
		<< "   nick:" << _clients.at(j).getNickname() 
		<< "   user:" <<_clients.at(j).getUsername() 
		<< "   realname:" <<_clients.at(j).getRealname() 
		<< std::endl;
	}
	std::cout << std::endl; 
	return ;
}

void	Server::newClient()
{
	int new_fd;
	char hostname[2048];
	sockaddr_in s_address = {};
	socklen_t s_size = sizeof(s_address);

	new_fd = accept(_sock, (sockaddr *) &s_address, &s_size);
	if (new_fd < 0)
		throw std::runtime_error("Error while accepting new client.");

	if (getnameinfo((struct sockaddr *) &s_address, sizeof(s_address), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV) !=
		0)
		throw std::runtime_error("Error while getting hostname on new client.");
	_clients.push_back(Client(new_fd, hostname));
	pollfd pollfd = {new_fd, POLLIN, 0};
	_pollfds.push_back(pollfd);
}

void	Server::eraseClient(int fd)
{
	std::vector<Client>::iterator it = _clients.begin();

	while (it != _clients.end())
	{
		if (it->getFd() == fd)
		{
			_clients.erase(it);
			return ;
		}
		it++;
	}
}

void	Server::clientDisconnect(int fd)
{
	std::vector<pollfd>::iterator it = _pollfds.begin();
	eraseClientChannel(findClient(fd));
	eraseClient(fd);
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
	std::cout << "Disconnection Successful" << std::endl;
}

String	Server::readMsg(int fd) 
{
	String	msg;
	char	buff[256];
	bzero(buff, 256);
	std::vector<Client>::iterator cl = findClientIt(fd);
	msg = cl->getMsg();

	while (!std::strstr(buff, "\n"))
	{
		int k = 0;
		bzero(buff, 256);
		if ((k = recv(fd, buff, 256, MSG_DONTWAIT)) < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("error in reciving message");
			return ("");
		}
		else if (k == 0)
		{
			throw(std::out_of_range("TEST DECO"));
		}
		cl->addMsg(buff);
		msg += buff;
	}
	cl->setMsg("");
	return msg;
}

void	Server::handleMessage(int fd) 
{
	try 
	{
		this->_cmd = splitCmd(readMsg(fd));
	}
	catch(const std::exception& e)
	{
		clientDisconnect(fd);
		std::cerr << e.what() << '\n';
		return ;
	}
	for (std::vector<String>::iterator it = this->_cmd.begin(); it != this->_cmd.end(); it++)
		parseCmd(*it, findClient(fd));
	displayClient();
}

std::vector<String>	Server::splitCmd(String msg) 
{
	std::vector<String> cmd;
	std::stringstream str(msg);
	String tmp;
	int i = 0;
	if (msg == "\n")
		return cmd;
	while (std::getline(str, tmp, '\n')) 
	{
		cmd.push_back(tmp);
		std::cout << cmd.at(i++) << std::endl;
	}
	return cmd;
}

void	Server::parseCmd(String str, Client &cl) 
{
	String tmp;
	std::vector<String>	args;
	std::stringstream ss(str);
	std::getline(ss, tmp, ' ');

	args.push_back(tmp);
  	std::cout << "Parse command : [" << tmp << "]" << std::endl;

	std::string cmds[15] = {"PASSWORD", "NICKNAME", "OPERATOR", "USER", "PRIVATE MESSAGE", "JOIN", "KILL", "PING", "PART", "LIST", "NAMES", "TOPIC", "KICK", "MODE", "NOTICE"};

	int		(Server::*ptr[15])(std::vector<String> args, Client &cl) = {
			&Server::cmdPass,
			&Server::cmdNick,
			&Server::cmdOper,
			&Server::cmdUser,
			&Server::cmdPrvMsg,
			&Server::cmdJoin,
			&Server::cmdKill,
			&Server::cmdPing,
			&Server::cmdPart,
            &Server::cmdList,
			&Server::cmdNames,
			&Server::cmdTopic,
			&Server::cmdKick,
			&Server::cmdMode,
			&Server::cmdNotice,

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

void	Server::launch()
{
	pollfd fd_server = {_sock, POLLIN, 0};
	_pollfds.push_back(fd_server);
	
	std::cout << "Server IRC Launched!" << std::endl;
	while (g_interrupt == false)
	{
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			break ;
		for (unsigned int i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents == 0)
				continue ;
			if ((_pollfds[i].revents  & POLLIN ) == POLLIN)
			{
				if (_pollfds[i].fd == _sock)
				{
					newClient();
					displayClient();
					break;
				}
			}
			handleMessage(_pollfds[i].fd);
		}
	}
	for (size_t i = 0; i < _pollfds.size(); i++)
		close(_pollfds[i].fd);
}

Client		&Server::findClient(int fd)
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getFd() == fd)
			return (_clients[i]);
	}
	throw(std::out_of_range("Error while searching for user"));
}

Client		&Server::findClient(String nick)
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (_clients[i].getNickname() == nick)
			return (_clients[i]);
	}
	throw(std::out_of_range("Error while searching for user"));
}

std::vector<Client>::iterator	Server::findClientIt(int fd)
{
	std::vector<Client>::iterator ret = _clients.begin();
	std::vector<Client>::iterator end = _clients.end();
	while (ret != end)
	{
		if (ret->getFd() == fd)
			return (ret);
		ret++;
	}
	throw(std::out_of_range("Error while searching for user"));
}

std::vector<Channel>::iterator Server::findChannelIt(std::string name)
{
	std::vector<Channel>::iterator  ret = _channels.begin();
	std::vector<Channel>::iterator  it_end = _channels.end();
	while (ret != it_end)
	{
		if (ret->getName() == name)
			return (ret);
		ret++;
	}
	throw (std::out_of_range("didnt find channel"));
}

Channel     &Server::findChannel(std::string name)
{
	for (unsigned int i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == name)
			return (_channels[i]);
	}
	throw (std::out_of_range("didnt find channel"));
}

void    Server::eraseClientChannel(Client &cl)
{
	for (unsigned int i = 0; i < _channels.size(); i++)
	{
		_channels[i].eraseClient(cl);
	}
	std::vector<Channel>::iterator   it = _channels.begin();
	while (it != _channels.end())
	{
		if (it->getClients().empty())
			it = _channels.erase(it);
		else
			it++;
	}
	std::cout << "eraseClientChannel" << std::endl;
}


/* ***************************** CHANNEL CLASS ****************************** */

Channel::Channel(String Name) : _name(Name), _topic(), _fdOp(0), _limit(0), _password("") {}

Channel::~Channel(){}

std::vector<Client>		&Channel::getClients(){return _clients;}
String					Channel::getName() const {return _name;}
String					Channel::getTopic() const {return _topic;}
int						Channel::getFdOp() const {return _fdOp;}
size_t     				Channel::getLimit() const {return _limit;}
String					Channel::getPassword() const {return _password;}

void					Channel::setTopic(String newTopic) {_topic = newTopic;}
void					Channel::setFdOp(int fd) {_fdOp = fd;}
void					Channel::setPassword(String pass) {_password = pass;}
void					Channel::setLimit(size_t limit) {_limit = limit;}
void					Channel::addClient(Client &cl) {_clients.push_back(cl);}

std::string     RPL_PART(std::string prefix, std::string name_chan) 
{
	return (prefix + " PART :" + name_chan);
}

void    Channel::eraseClient(Client &cl)
{
	std::vector<Client>::iterator   it;
	for(it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << it->getNickname() << "==" << cl.getNickname() << std::endl;
		if (it->getFd() == cl.getFd())
		{
			std::cout << "erasing client" << std::endl;
			broadcast(RPL_PART(cl.getPrefix(), _name));
			_clients.erase(it);
			return ;
		}
	}
	std::cout << "not really erasing client" << _clients.size() << std::endl;
}

void	Channel::broadcast(std::string message)
{
	message += "\r\n";
	std::cout << "----> " << message << std::endl;
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (send(_clients[i].getFd(), message.c_str(), message.length(), 0) < 0)
			throw std::out_of_range("error while broadcasting");
	}
}

void	Channel::broadcast(std::string message, Client &cl)
{
	message += "\r\n";
	std::cout << "----> "<< message << std::endl;
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (cl.getFd() != _clients[i].getFd())
		{
			if (send(_clients[i].getFd(), message.c_str(), message.length(), 0) < 0)
				throw std::out_of_range("error while broadcasting");
		}
	}
}

void	Channel::debug()
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		std::cout << "#client " << i << " " << _clients[i].getNickname() << std::endl;
	}
}


/* ***************************** CLIENT CLASS ******************************* */

Client::Client(int sockfd, std::string hostname) : _sockfd(sockfd), _hostname(hostname), _isoper(false)
{
    _state = HANDSHAKE;
    _msg = "";
}

Client::~Client() {}

String  Client::getPrefix()
{
    String prefix = ":" + _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
    return prefix;
}

void    Client::reply(String msg) 
{
    String prefix = _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
    String paquet = ":" + prefix + " " + msg + "\r\n";
    std::cout << "---> " << paquet << std::endl;
    if (send(_sockfd, paquet.c_str(), paquet.length(), 0) < 0)
        throw(std::out_of_range("Error while sending"));
}

void    Client::welcome() 
{
    if (_state != LOGIN || _nickname.empty() || _username.empty())
    {
        std::cout << "Waiting registration... " << _nickname << std::endl;
        return ;
    }
    _state = REGISTERED;
    reply("001 " + _nickname + " :Welcome " +_nickname +  " into our irc network");
    std::cout << _nickname << " is registered" << std::endl;
}

int     Client::getFd() const {return _sockfd;}
String  Client::getNickname() const {return _nickname;}
String  Client::getUsername() const {return _username;}
String  Client::getRealname() const {return _realname;}
String  Client::getHostname() const {return _hostname;}
String  Client::getMsg() const {return _msg;}
State	Client::getState() const {return _state;}
bool	Client::getisoper() const {return _isoper;}

void	Client::setNickname(String newName) {_nickname = newName;}
void	Client::setUsername(String newName) {_username = newName;}
void	Client::setRealname(String newName) {_realname = newName;}
void	Client::setHostname(String newName) {_hostname = newName;}
void	Client::setMsg(String newMsg) {_msg = newMsg;}
void    Client::addMsg(std::string buff) 
{
    _msg += buff;
}
void	Client::setisoper(bool isoper) {_isoper = isoper;}

void  Client::setState(State new_state)
{
    _state = new_state;
}