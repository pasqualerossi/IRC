/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_users.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:28:09 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:39:26 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::user_command(std::vector<String> args, Client &client) 
{
	String temporary;
	String command = args.at(0);
	if (args.size() < 5)
	{
		client.reply(error_need_more_Parameters(client, "USER"));
		return -1;
	}
	else if (client.get_User_name() == args.at(1))
	{
		client.reply("462 " + client.get_Nick_name() + " " + command + " :Unauthorized command (already registered)");
		return -1;
	}
	else if (args.size() >= 5)
	{
		client.set_Username(args.at(1));
		size_t i;
		
		for (i = 1; i < args.size() - 1; i++)
		{
			if (args.at(i)[0] == ':')
				break ;
		}
		
		if (args.at(1)[0] == ':')
		{
			temporary = args.at(i).substr(1);
		}
		else
		{
			temporary = args.at(i);
		}
		
		if (i + 1 == args.size())
		{
			client.set_Real_Name(temporary);
			client.welcome_message();
			return 0;
		}
		i++;
		temporary += " ";
		
		for (; i < args.size() - 1; i++)
		{
			temporary += args.at(i) + " ";
		}
		temporary += args.at(i);
		client.set_Real_Name(temporary);
	}
	client.welcome_message();
	return 0;
}