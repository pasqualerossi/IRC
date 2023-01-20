/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_nicknames.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:13 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:39:30 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

bool	Server::server_already_used(String name, Client client)
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->get_Nick_name() == name && it->get_file_descriptor() != client.get_file_descriptor())
		{
			return (true);
		}
	}
	return (false);
}

bool	special_characters(char characters) 
{
	if (characters >= 0x5B && characters <= 0x60)
	{
		return (true);
	}
	else if (characters >= 0x7B && characters <= 0x7D)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool	valid_nickname(String name) 
{
	if (!(special_characters(name[0]) || isalpha(name[0])))
	{
		return (false);
	}
	for (size_t i = 1; i < name.size(); ++i)
	{
		if (!(isalnum(name[i]) || special_characters(name[i]) || name[i] == '-'))
		{
			return (false);
		}
	}
	return (true);
}

String	error_no_nickname_given(Client client) 
{
	return ("431 " + client.get_Nick_name() + " :No nickname given");
}

String	error_wrong_nickname(Client client, String new_nickname) 
{
	return ("432 " + client.get_Nick_name() + " " + new_nickname + " :wrong nickname");
}

String	error_nickname_in_use(Client client, String new_nickname) 
{
	return ("433 " + client.get_Nick_name() + " " + new_nickname + " " + new_nickname + " :Nickname is already in use");
}

String	success_nickname(String new_nickname) 
{
	return (" NICK " + new_nickname);
}

int Server::nickname_Command(std::vector<String> arguments, Client &client)
{
	String new_nickname = carriage_return(arguments[1]);

	if (arguments.size() < 2)
	{
		client.reply(error_no_nickname_given(client));
		return (-1);
	}
	if (valid_nickname(new_nickname) == false)
	{
		client.reply(error_wrong_nickname(client, new_nickname));
		return (-1);
	}
	if (server_already_used(new_nickname, client) == true)
	{
		client.reply(error_nickname_in_use(client, new_nickname));
		return (-1);
	}
    client.reply(success_nickname(new_nickname));
	client.set_Nickname(new_nickname);
	client.welcome_message();
	return 0;
}