/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:29:39 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 19:13:26 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

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

String	error_need_more_Parameters(Client &client, String command) 
{
	return ("461 " + client.get_Nick_name() + " " + command + " :Not enough parameters");
}

String  error_no_such_Channel(Client client, String channel) 
{
	return ("403 " + client.get_Nick_name() + " " + channel +" :No such channel");
}

String  error_channel_operator_is_Needed(Client client, String channel) 
{
	return ("482 " + client.get_Nick_name() + " " + channel + " :You're not channel operator");
}

String  rpl_topic_Message(Client client, String channel, String topic) 
{
	return ("332 " + client.get_Nick_name() + " " + channel + " " + topic);
}

bool	is_Client_In_Channel(Channel &channel, int fd)
{
	for (unsigned int i = 0; i < channel.get_Clients().size(); i++)
	{
		if (channel.get_Clients()[i].get_file_descriptor() == fd)
		{
			return true;
		}
	}
	return false;
}

bool	is_Client_Not_In_Channel(Channel &channel, String name)
{
	for (unsigned int i = 0; i < channel.get_Clients().size(); i++)
	{
		if (channel.get_Clients()[i].get_Nick_name() == name)
		{
			return true;
		}
	}
	return false;
}

bool	is_Operator_In_Channel(Client client, Channel channel)
{
	if (channel.get_file_descriptorOp() == client.get_file_descriptor())
		return true;
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