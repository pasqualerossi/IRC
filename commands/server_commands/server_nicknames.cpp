/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_nicknames.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:13 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

bool	Server::server_already_used(String name, Client client_side)
{
	for (std::vector<Client>::iterator it = this->client_side.begin(); it != this->_clients.end(); ++it)
	{
		if (it->get_Nick_name() == name && it->getFd() != client_side.getFd())
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

String	error_no_nickname_given(Client client_side) 
{
	return ("431 " + client_side.get_Nick_name() + " :No nickname given");
}

String	error_wrong_nickname(Client client_side, String new_nickname) 
{
	return ("432 " + client_side.get_Nick_name() + " " + new_nickname + " :wrong nickname");
}

String	error_nickname_in_use(Client client_side, String new_nickname) 
{
	return ("433 " + client_side.get_Nick_name() + " " + new_nickname + " " + new_nickname + " :Nickname is already in use");
}

String	success_nickname(String new_nickname) 
{
	return (" NICK " + new_nickname);
}

int Server::nickname_Command(std::vector<String> arguments, Client &client_side)
{
	String new_nickname = carriage_return(arguments[1]);

	if (arguments.size() < 2)
	{
		client_side.reply_to_message(error_no_nickname_given(client_side));
		return (-1);
	}
	if (valid_nickname(new_nickname) == false)
	{
		client_side.reply_to_message(error_wrong_nickname(client_side, new_nickname));
		return (-1);
	}
	if (server_already_used(new_nickname, client_side) == true)
	{
		client_side.reply_to_message(error_nickname_in_use(client_side, new_nickname));
		return (-1);
	}
    client_side.reply_to_message(success_nickname(new_nickname));
	client_side.set_Nickname(new_nickname);
	client_side.welcome_message();
	return 0;
}