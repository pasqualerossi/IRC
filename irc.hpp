/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:21:09 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:36 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
# define IRC_HPP

// libraries

# include <iostream>
# include <vector>
# include <sstream>
# include <sys/socket.h>
# include <sys/poll.h>
# include <sys/types.h>
# include <arpa/inet.h>
# include <algorithm>
# include <netdb.h>
# include <fcntl.h>
# include <vector>
# include <map>
# include <poll.h>
# include <unistd.h>
# include <cstdlib>
# include <string>
# include <strings.h>
# include <cstring>
# include <cerrno>
# include <signal.h>

// external

typedef std::string String;

extern bool g_interrupt;

// enum

enum State
{
	HANDSHAKE,
	LOGIN,
	REGISTERED
};

/* 3 Classes - Client, Server and Channel */

typedef std::string String;

class Channel;

class Client 
{
	private:

		int	_socket_fd;

		bool _is_operator;

		String _nickname;
		String _username;
		String _real_name;
		String _hostname;
		String _message;
		State _state;

		std::vector<Channel> _channels;

	public:

		Client(int fd, String host);
		~Client();

		String get_Nick_name() const;
		String get_User_name() const;
		String get_Real_Name() const;
		String get_Hostname() const;
		String get_Message() const;
		String get_Prefix();
		State get_State() const;

		int getFd() const;

		bool get_operator() const;

		void set_Nickname(String new_Name);
		void set_Username(String new_Name);
		void set_Real_Name(String new_Name);
		void set_Hostname(String new_Name);
		void set_Message(String new_Message);
		void add_Message(String buffer);
		void set_State(State new_State);
		void set_is_operator(bool is_operator);
		void reply_to_message(String message);
		void welcome_message();
};

// Server Class

typedef std::string String;

extern bool g_interrupt;

class Client;
class Channel;

class Server 
{
	private:

		String _host;
		String _password;
		String _operator_password;
		
		int _port;
		int _socket;

		std::vector<String> _command;
		std::vector<pollfd>	_pollfds;
		std::vector<Client>	_clients;
		std::vector<Channel> _channels;

	public:

		Server(int port, const String &password);
		~Server();

		void launch_Server();
		void parse_Command(String str, Client &client_side);
		void handle_Message(int fd);
		void display_the_Client();
		void adding_the_new_Client();
		void erase_the_Client(int fd);
		void erase_Client_Channel(Client &client_side);
		void client_Disconnect(int fd);

		std::vector<String>	split_Command(String msg);
		std::vector<Client>::iterator 		find_a_Client(int fd);
		std::vector<Channel>::iterator		find_a_Channel(String name);

		bool server_already_used(String name, Client client_side);
		bool this_Channel(std::string name);

		String read_Message(int fd);
		Client &find_Client(int fd);
		Client &find_Client(String nickname);
		Channel	&find_Channel(String name);

		int	create_socket();
		int	change_Message(std::vector<String> parameters, Client &client_side);
		int	change_Notice(std::vector<String> parameters, Client &client_side);
		int channel_Names(std::vector<String> arguments, Client &client_side);
		int	command_List(std::vector<String> arguments, Client &client_side);
		int	password_Command(std::vector<String> password, Client &client_side);
		int	nickname_Command(std::vector<String> password, Client &client_side);
		int	user_Command(std::vector<String> password, Client &client_side);
		int	private_message_Command(std::vector<String> password, Client &client_side);
		int	ping_command(std::vector<String> arguments, Client &client_side);
		int	join_Command(std::vector<String> arguments, Client &client_side);
		int	operator_command(std::vector<String> arguments, Client &client_side);
		int	kill_Command(std::vector<String> arguments, Client &client_side);
		int	part_Command(std::vector<String> arguments, Client &client_side);
		int	topic_command(std::vector<String> arguments, Client &client_side);
		int	kick_Command(std::vector<String> arguments, Client &client_side);
		int	notice_Command(std::vector<String> arguments, Client &client_side);
		int	mode_Command(std::vector<String> arguments, Client &client_side);
};

typedef std::string String;

class Client;

class Channel 
{
	private:
		
		String 					_name;
		String 					_topic;
		String 					_password;

		int 					_file_descriptor_with_zero_permissions;

		size_t 					_limit;

		std::vector<Client>		_clients;

	public:
		
		Channel(std::string _name);
		~Channel();

		int getFd_with_zero_permissions() const;

		size_t get_Limit() const;

		String get_Name() const;
		String get_Topic() const;
		String get_Password() const;
		
		std::vector<Client>	&get_Clients();

		void set_Topic(String newTopic);
		void set_Password(String password);
		void set_file_descriptor_to_zero_permissions(int fd);
		void set_Limit(size_t limit);
		void add_Client(Client &client_side);
		void erase_the_Client(Client &client_side);
		void broadcast_message(std::string message);
		void broadcast_message(std::string message, Client &client_side);
		void debug_channel();
};

// Utils Functions

String carriage_return(String string);
String error_need_more_Parameters(Client &client_side, String command);
String error_no_such_Channel(Client client_side, String channel_side);
String error_channel_operator_is_Needed(Client client_side, String channel_side);
String rpl_topic_Message(Client client_side, String channel_side, String topic);
	
bool is_Client_In_Channel(Channel &channel_side, int fd);
bool is_Client_Not_In_Channel(Channel &channel_side, String name);
bool is_Operator_In_Channel(Client client_side, Channel channel_side);

std::vector<String> split(String string, const char *delimiter);

#endif