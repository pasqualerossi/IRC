/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/01 12:37:53 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

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

