/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_channels.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:30 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

bool     Server::this_Channel(std::string name)
{
	for (unsigned int i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].get_Name() == name)
			return true;
	}
	return false;
}

int	Server::kick_Command(std::vector<String> arguments, Client &client_side) 
{
	String command = arguments.at(0);

	if (arguments.size() < 3)
	{
		client_side.reply_to_message("461 " + client_side.get_Nick_name() + " " + command + " :Not enough parameters");
		return -1;
	}
	if (this_Channel(arguments.at(1)) == false)
	{
		client_side.reply_to_message("403 " + client_side.get_Nick_name() + " " + arguments.at(1) + " :No such channel");
		return -1;
	}
	if (is_client_in_Channel(find_Channel(arguments.at(1)), client.getFd()) == false)
	{
		client_side.reply_to_message("442 " + client_side.get_Nick_name() + " " + arguments.at(1) + " :You're not on that channel");
		return -1;
	}
	if (client_side.getFd() != find_Channel(arguments.at(1)).getFd_with_zero_permissions())
	{
		client_side.reply_to_message(error_channel_operator_is_Needed(client_side, arguments.at(1)));
		return -1;
	}
	if (isClientNInChannel(find_Channel(arguments.at(1)), carriage_return(arguments.at(2))) == false)
	{
		client_side.reply_to_message("441 " + client_side.get_Nick_name() + " " + carriage_return(arguments.at(2)) + " " + arguments.at(1) + " :They aren't on that channel");
		return -1;
	}
	std::vector<String> temporary;
	temporary.push_back("PART");
	temporary.push_back(arguments.at(1));
	part_Command(temporary, find_Client(arguments.at(2)));
	return 0;
}