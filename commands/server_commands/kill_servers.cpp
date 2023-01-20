/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_servers.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:29:19 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:39:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int	Server::kill_Command(std::vector<String> arguments, Client &client)
{
	String command = arguments.at(0);
	size_t	i = 0;
	int		check = 0;

	if (client.get_operator() == false)
	{
		client.reply("481 " + client.get_Nick_name() + " " +  command + " :Permission Denied- You're not an IRC operator");
		return -1;
	}
	if (arguments.size() < 2)
	{
		client.reply("461 " + cl.get_Nick_name() + " " + command + " :Not enough parameters");
		return -1;
	}
	for (i = 0; i < _clients.size(); i++)
	{
		if (arguments.at(1) == _clients.at(i).get_Nick_name())
		{
			check = 1;
			break ;
		}
	}
	if (check != 1)
	{
		client.reply("401 " + client.get_Nick_name() + " " + arguments.at(1) + " :No such nick");
		return -1;
	}
	clientDisconnect(_clients.at(i).get_file_descriptor());
	return 0;
}