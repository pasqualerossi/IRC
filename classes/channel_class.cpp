/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_class.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:29:55 by prossi            #+#    #+#             */
/*   Updated: 2023/01/19 17:32:50 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// CHANNEL ACTIONS - erase, broadcast and debug the channel/s.

Channel::Channel(String Name) : _name(Name), _topic(), _file_descriptor_with_zero_permissions(0), _limit(0), _password("") {}

Channel::~Channel(){}

std::vector<Client>		&Channel::get_Clients(){return _clients;}
String					Channel::get_Name() const {return _name;}
String					Channel::get_Topic() const {return _topic;}
int						Channel::get_file_descriptor_with_zero_permissions() const {return _file_descriptor_with_zero_permissions;}
size_t     				Channel::get_Limit() const {return _limit;}
String					Channel::get_Password() const {return _password;}

void					Channel::set_Topic(String newTopic) {_topic = newTopic;}
void					Channel::set_file_descriptor_to_zero_permissions(int fd) {_file_descriptor_with_zero_permissions = fd;}
void					Channel::set_Password(String pass) {_password = pass;}
void					Channel::set_Limit(size_t limit) {_limit = limit;}
void					Channel::add_Client(Client &cl) {_clients.push_back(cl);}

// CHANNEL CLIENT

std::string     RPL_PART(std::string prefix, std::string name_change) 
{
	return (prefix + " PART :" + name_change);
}

void    Channel::erase_the_Client(Client &client)
{
	std::vector<Client>::iterator   it;
	for(it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << it->get_Nick_name() << "==" << client.get_Nick_name() << std::endl;
		if (it->get_file_descriptor() == client.get_file_descriptor())
		{
			std::cout << "erasing client" << std::endl;
			broadcast_message(RPL_PART(client.get_Prefix(), _name));
			_clients.erase(it);
			return ;
		}
	}
	std::cout << "not realy erasing client" << _clients.size() << std::endl;
}

// CHANNEL broadcast_message

void	Channel::broadcast_message(std::string message)
{
	message += "\r\n";
	std::cout << "----> " << message << std::endl;
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (send(_clients[i].get_file_descriptor(), message.c_str(), message.length(), 0) < 0)
		{
			throw std::out_of_range("error while broadcast_messageing");
		}
	}
}

void	Channel::broadcast_message(std::string message, Client &client)
{
	message += "\r\n";
	std::cout << "----> "<< message << std::endl;
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		if (client.get_file_descriptor() != _clients[i].get_file_descriptor())
		{
			if (send(_clients[i].get_file_descriptor(), message.c_str(), message.length(), 0) < 0)
			{
				throw std::out_of_range("error while broadcast_messageing");
			}
		}
	}
}

// CHANNEL debug_channel

void	Channel::debug_channel()
{
	for (unsigned int i = 0; i < _clients.size(); i++)
	{
		std::cout << "#client " << i << " " << _clients[i].get_Nick_name() << std::endl;
	}
}