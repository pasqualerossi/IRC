/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:29:39 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:27:15 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

String	carriage_return(String string)
{
	if (string.empty())
	{
		return "";
	}
	if (string[string.size() - 1] == '\r')
	{
		return string.substr(0, (string.size() - 1));
	}
	return string;
}

String	error_need_more_Parameters(Client &client_side, String command) 
{
	return ("461 " + client_side.get_Nick_name() + " " + command + " :Not enough parameters");
}

String  error_no_such_Channel(Client client_side, String channel_side) 
{
	return ("403 " + client_side.get_Nick_name() + " " + channel_side +" :No such channel");
}

String  error_channel_operator_is_Needed(Client client_side, String channel_side) 
{
	return ("482 " + client_side.get_Nick_name() + " " + channel_side + " :You're not channel operator");
}

String  rpl_topic_Message(Client client_side, String channel_side, String topic) 
{
	return ("332 " + client_side.get_Nick_name() + " " + channel_side + " " + topic);
}

bool	is_Client_In_Channel(Channel &channel_side, int fd)
{
	for (unsigned int i = 0; i < channel_side.get_Clients().size(); i++)
	{
		if (channel_side.get_Clients()[i].getFd() == fd)
		{
			return true;
		}
	}
	return false;
}

bool	is_Client_Not_In_Channel(Channel &channel_side, String name)
{
	for (unsigned int i = 0; i < channel_side.get_Clients().size(); i++)
	{
		if (channel_side.get_Clients()[i].get_Nick_name() == name)
		{
			return true;
		}
	}
	return false;
}

bool	is_Operator_In_Channel(Client client_side, Channel channel_side)
{
	if (channel_side.getFd_with_zero_permissions() == client_side.getFd())
	{
		return true;
	}
	return false;
}

std::vector<String> split(String string, const char *delimiter)
{
	std::vector<String> list;

	char 	*pointer = strtok((char *)string.c_str(), delimiter);

	while (pointer)
	{
		list.push_back(std::string(pointer));
		pointer = strtok(NULL, delimiter);
	}
	return list;
}