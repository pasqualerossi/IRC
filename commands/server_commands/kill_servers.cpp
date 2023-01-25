/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_servers.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:29:19 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int	Server::kill_Command(std::vector<String> arguments, Client &client_side)
{
	String command = arguments.at(0);
	size_t	i = 0;
	int		check = 0;
	if (client_side.get_operator() == false)
	{
		client_side.reply_to_message("481 " + client_side.get_Nick_name() + " " +  command + " :Permission Denied- You're not an IRC operator");
		return -1;
	}
	if (arguments.size() < 2)
	{
		client_side.reply_to_message("461 " + client_side.get_Nick_name() + " " + command + " :Not enough parameters");
		return -1;
	}
	for (i = 0; i < client_side.size(); i++)
	{
		if (arguments.at(1) == _clients.at(i).get_Nick_name())
		{
			check = 1;
			break ;
		}
	}
	if (check != 1)
	{
		client_side.reply_to_message("401 " + client_side.get_Nick_name() + " " + arguments.at(1) + " :No such nick");
		return -1;
	}
	client_Disconnect(_clients.at(i).getFd());
	return 0;
}