/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/01 12:42:05 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

String	erasebr(String str) 
{
	if (str.empty())
		return "";
	if (str[str.size() - 1] == '\r')
		return str.substr(0, (str.size() - 1));
	return str;
}

String	ERROR_NEED_MORE_PARAMETERS(Client &client, String cmd) 
{
	return ("461 " + client.getNickname() + " " + cmd + " :Not enough parameters");
}

String  ERROR_NO_SUCH_CHANNEL_EXISTS(Client cl, String channel) 
{
	return ("403 " + cl.getNickname() + " " + channel +" :No such channel");
}

String  ERROR_CHANNEL_OPERATOR_NEEDED(Client cl, String channel) 
{
	return ("482 " + cl.getNickname() + " " + channel + " :You're not channel operator");
}

String  RPL_TOPIC(Client cl, String channel, String topic) 
{
	return ("332 " + cl.getNickname() + " " + channel + " " + topic);
}

bool	isClientInChannel(Channel &chan, int fd)
{
	for (unsigned int i = 0; i < chan.getClients().size(); i++)
	{
		if (chan.getClients()[i].getFd() == fd)
			return true;
	}
	return false;
}

bool	is_client_not_in_Channel(Channel &chan, String name)
{
	for (unsigned int i = 0; i < chan.getClients().size(); i++)
	{
		if (chan.getClients()[i].getNickname() == name)
			return true;
	}
	return false;
}

bool	is_operator_in_Channel(Client cl, Channel chan)
{
	if (chan.getFdOp() == cl.getFd())
		return true;
	return false;
}

std::vector<String> split(String str, const char *delim)
{
	std::vector<String> list;

	char 	*ptr = strtok((char *)str.c_str(), delim);

	while (ptr)
	{
		list.push_back(std::string(ptr));
		ptr = strtok(NULL, delim);
	}
	return list;
}