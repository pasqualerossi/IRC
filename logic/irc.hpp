/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/01 12:37:16 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

// Libraries

# include <iostream>
# include <vector>
# include <sstream>
# include <sys/socket.h>
# include <algorithm>
# include <sys/types.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <fcntl.h>
# include <map>
# include <poll.h>
# include <unistd.h>
# include <cstdlib>
# include <string>
# include <strings.h>
# include <cstring>
# include <cerrno>
# include <signal.h>

// Extern Bool

extern bool g_interrupt;

// Typedef

typedef std::string String;

// enum

enum State
{
	HANDSHAKE,
	LOGIN,
	REGISTERED
};

/* Classes - Client, Channel and Server */


// Client Class

class Channel;

class Client 
{
	private:
		int			_sockfd;
		String		_nickname;
		String		_username;
		String		_realname;
		String		_hostname;
		String		_msg;
		State		_state;
		bool		_isoper;
		std::vector<Channel>	_channels;

	public:
		Client(int fd, String host);
		~Client();

	// Client functions

		void		reply(String msg);
		void		welcome();

	// Getter functions

		int			getFd() const;
		String		getNickname() const;
		String		getUsername() const;
		String		getRealname() const;
		String		getHostname() const;
		String		getMsg() const;
		String		getPrefix();
		State		getState() const;
		bool		getisoper() const;

	// Setter functions
	
		void		setNickname(String newName);
		void		setUsername(String newName);
		void		setRealname(String newName);
		void		setHostname(String newName);
		void		setMsg(String newMsg);
		void		addMsg(String buff);
		void		setState(State newState);
		void		setisoper(bool isoper);
};

// Channel Class

class Channel 
{
	private:
		String						_name;
		String						_topic;
		int							_fdOp;
		size_t						_limit;
		String						_password;
		std::vector<Client>			_clients;

	public:
		Channel(std::string _name);
		~Channel();

	// Getter

		int						getFdOp() const; 
		size_t					getLimit() const;
		String					getName() const;
		String					getTopic() const;
		String					getPassword() const;
		std::vector<Client>		&getClients();

	// Setter

		void					setTopic(String newTopic);
		void					setPassword(String pass);
		void					setFdOp(int fd);
		void					setLimit(size_t limit);

	// Channel functions
	
		void					addClient(Client &client);
		void					eraseClient(Client &cl);
		void					broadcast(std::string message);
		void					broadcast(std::string message, Client &cl);
		void					debug();
};

// Server Class

class Server 
{
	private:
		String								_host;
		String								_password;
		String								_operPassword;
		int									_port;
		int									_sock;

		std::vector<String>					_cmd;
		std::vector<pollfd>					_pollfds;
		std::vector<Client>					_clients;
		std::vector<Channel>				_channels;

	public:
		Server(int port, const String &password);
		~Server();

	// Server Init

		int									createSocket();
		void								launch();

	// Server Display

		void								handleMessage(int fd);
		void								displayClient();

	// Server Receipt 

		std::vector<String>					splitCmd(String msg);
		void								parseCmd(String str, Client &cl);
		String								readMsg(int fd);

	// Manage Clients

		void								newClient();
		void								eraseClient(int fd);
		void								eraseClientChannel(Client &cl);
		void								clientDisconnect(int fd);

	// Server Utils

		int									chanMessage(std::vector<String> params, Client &cl);
		int									chanNotice(std::vector<String> params, Client &cl);
		bool								already_used(String name, Client cl);

		Client								&findClient(int fd);
		Client								&findClient(String nickname);
		std::vector<Client>::iterator		findClientIt(int fd);
		

		bool     							isChannel(std::string name);
		Channel								&findChannel(String name);
		std::vector<Channel>::iterator		findChannelIt(String name);

	// IRC Commands

		int									cmdPass(std::vector<String> pass, Client &cl);
		int									cmdNick(std::vector<String> pass, Client &cl);
		int									cmdUser(std::vector<String> pass, Client &cl);
		int									cmdPrvMsg(std::vector<String> pass, Client &cl);
		int									cmdPing(std::vector<String> args, Client &cl);
		int									cmdJoin(std::vector<String> args, Client &cl);
		int									cmdOper(std::vector<String> args, Client &cl);
		int									cmdKill(std::vector<String> args, Client &cl);
		int									cmdPart(std::vector<String> args, Client &cl);
		int									cmdList(std::vector<String> args, Client &cl);
		int									cmdNames(std::vector<String> args, Client &cl);
		int									cmdTopic(std::vector<String> args, Client &cl);
		int									cmdKick(std::vector<String> args, Client &cl);
		int									cmdNotice(std::vector<String> args, Client &cl);
		int									cmdMode(std::vector<String> args, Client &cl);
};

// Utils.cpp

	String								erasebr(String str);
	String								ERROR_NEED_MORE_PARAMETERS(Client &client, String cmd);
	String								ERROR_NO_SUCH_CHANNEL_EXISTS(Client cl, String channel);
	String								ERROR_CHANNEL_OPERATOR_NEEDED(Client cl, String channel);
	String								RPL_TOPIC(Client cl, String channel, String topic);
	
	bool								isClientInChannel(Channel &chan, int fd);
	bool								is_client_not_in_Channel(Channel &chan, String name);
	bool								is_operator_in_Channel(Client cl, Channel chan);

	std::vector<String> 				split(String str, const char *delim);

#endif
