/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_class.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:30:07 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// CLIENT ACTIONS - reply_to_message, welcome, message and get operator for the client. 

Client::Client(int sockfd, std::string hostname) : _socket_fd(sockfd), _hostname(hostname), _is_operator(false)
{
    _state = HANDSHAKE;
    _message = "";
}

Client::~Client() {}

String  Client::get_Prefix()
{
    String prefix = ":" + _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
    return prefix;
}

void    Client::reply_to_message_to_message(String message) 
{
    String prefix = _nickname + (_username.empty() ? "" : "!" + _username) + (_hostname.empty() ? "" : "@" + _hostname);
    String packet = ":" + prefix + " " + message + "\r\n";
    
	std::cout << "---> " << packet << std::endl;
    if (send(_socket_fd, packet.c_str(), packet.length(), 0) < 0)
	{
        throw(std::out_of_range("Error while sending"));
	}
}

void    Client::welcome_message() 
{
    if (_state != LOGIN || _nickname.empty() || _username.empty())
    {
        std::cout << "Waiting registration... " << _nickname << std::endl;
        return ;
    }
    _state = REGISTERED;
    reply_to_message_to_message("001 " + _nickname + " :Welcome " +_nickname +  " into our irc network");
    std::cout << _nickname << " is registered" << std::endl;
}

int     Client::getFd() const {return _socket_fd;}
String  Client::get_Nick_name() const {return _nickname;}
String  Client::get_User_name() const {return _username;}
String  Client::get_Real_Name() const {return _real_name;}
String  Client::get_Hostname() const {return _hostname;}
String  Client::get_Message() const {return _message;}
State	Client::get_State() const {return _state;}
bool	Client::get_operator() const {return _is_operator;}

void	Client::set_Nickname(String new_Name) {_nickname = new_Name;}
void	Client::set_Username(String new_Name) {_username = new_Name;}
void	Client::set_Real_Name(String new_Name) {_real_name = new_Name;}
void	Client::set_Hostname(String new_Name) {_hostname = new_Name;}
void	Client::set_Message(String new_Message) {_message = new_Message;}

void    Client::add_Message(std::string buffer) 
{
    _message += buffer;
}

void	Client::set_is_operator(bool is_operator) {_is_operator = is_operator;}

void  Client::set_State(State new_state)
{
    _state = new_state;
}