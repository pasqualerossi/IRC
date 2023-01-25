/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_users.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:28:09 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::user_command(std::vector<String> arguments, Client &client_side) 
{
	String temporary;
	String command = arguments.at(0);
	if (arguments.size() < 5)
	{
		client_side.reply_to_message(error_need_more_Parameters(client_side, "USER"));
		return -1;
	}
	else if (client_side.get_User_name() == arguments.at(1))
	{
		client_side.reply_to_message("462 " + client_side.get_Nick_name() + " " + command + " :Unauthorized command (already registered)");
		return -1;
	}
	else if (arguments.size() >= 5)
	{
		client_side.set_Username(arguments.at(1));
		size_t i;
		
		for (i = 1; i < arguments.size() - 1; i++)
		{
			if (arguments.at(i)[0] == ':')
				break ;
		}
		
		if (arguments.at(1)[0] == ':')
		{
			temporary = arguments.at(i).substr(1);
		}
		else
		{
			temporary = arguments.at(i);
		}
		
		if (i + 1 == arguments.size())
		{
			client_side.set_Real_Name(temporary);
			client_side.welcome_message();
			return 0;
		}
		i++;
		temporary += " ";
		
		for (; i < arguments.size() - 1; i++)
		{
			temporary += arguments.at(i) + " ";
		}
		temporary += arguments.at(i);
		client_side.set_Real_Name(temporary);
	}
	client_side.welcome_message();
	return 0;
}