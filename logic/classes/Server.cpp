#include "../irc.hpp"

Server::Server(int port, const String &pass)
	: _host("127.0.0.1"), _password(pass), _operPassword("operpass"), _port(port) 
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
	std::cout << "disconnexion succefull" << std::endl;
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
				throw std::runtime_error("error in recv");
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
	while (std::getline(str, tmp, '\n')) {
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

	std::string cmds[15] = {"PASS", "NICK", "OPER", "USER", "PRIVMSG", "JOIN", "kill", "PING", "PART", "LIST", "NAMES", "TOPIC", "KICK", "MODE", "NOTICE"};

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
	
	std::cout << "Server IRC launched !" << std::endl;
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