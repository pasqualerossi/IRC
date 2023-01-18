/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill_servers.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:29:19 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 18:36:47 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

int	Server::kill_Command(std::vector<String> args, Client &cl)
{
	String cmd = args.at(0);
	size_t	i = 0;
	int		check = 0;

	if (cl.get_operator() == false)
	{
		cl.reply("481 " + cl.get_Nick_name() + " " +  cmd + " :Permission Denied- You're not an IRC operator");
		return -1;
	}
	if (args.size() < 2)
	{
		cl.reply("461 " + cl.get_Nick_name() + " " + cmd + " :Not enough parameters");
		return -1;
	}
	for (i = 0; i < _clients.size(); i++)
	{
		if (args.at(1) == _clients.at(i).get_Nick_name())
		{
			check = 1;
			break ;
		}
	}
	if (check != 1)
	{
		cl.reply("401 " + cl.get_Nick_name() + " " + args.at(1) + " :No such nick");
		return -1;
	}
	clientDisconnect(_clients.at(i).get_file_descriptor());
	return 0;
}