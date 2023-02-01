/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICKNAME.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/01 12:38:39 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

bool	Server::already_used(String name, Client cl)
{
	for (std::vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); ++it)
	{
		if (it->getNickname() == name && it->getFd() != cl.getFd())
			return (true);
	}
	return (false);
}

bool	isSpecial(char c) 
{
	if (c >= 0x5B && c <= 0x60)
		return (true);
	else if (c >= 0x7B && c <= 0x7D)
		return (true);
	else
		return (false);
}

bool	valid_nickname(String name) 
{
	if (!(isSpecial(name[0]) || isalpha(name[0])))
		return (false);
	for (size_t i = 1; i < name.size(); ++i)
	{
		if (!(isalnum(name[i]) || isSpecial(name[i]) || name[i] == '-'))
			return (false);
	}
	return (true);
}

String	ERR_NONICKNAMEGIVEN(Client cl) 
{
	return ("431 " + cl.getNickname() + " :No nickname given");
}

String	ERR_ERRONEUSNICKNAME(Client cl, String newNick) 
{
	return ("432 " + cl.getNickname() + " " + newNick + " :Erroneus nickname");
}

String	ERR_NICKNAMEINUSE(Client cl, String newNick) 
{
	return ("433 " + cl.getNickname() + " " + newNick + " " + newNick + " :Nickname is already in use");
}

String	SUCCESS_NICK(String newNick) {return (" NICK " + newNick);}

int Server::cmdNick(std::vector<String> args, Client &cl)
{
	if (args.size() < 2)
	{
		cl.reply(ERR_NONICKNAMEGIVEN(cl));
		return (-1);
	}
	String newNick = erasebr(args[1]);
	if (valid_nickname(newNick) == false)
	{
		cl.reply(ERR_ERRONEUSNICKNAME(cl, newNick));
		return (-1);
	}
	if (already_used(newNick, cl) == true)
	{
		cl.reply(ERR_NICKNAMEINUSE(cl, newNick));
		return (-1);
	}
    cl.reply(SUCCESS_NICK(newNick));
	cl.setNickname(newNick);
	cl.welcome();
	return 0;
}