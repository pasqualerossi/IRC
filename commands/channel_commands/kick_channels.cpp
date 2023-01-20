/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_channels.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:30 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:48:22 by prossi           ###   ########.fr       */
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

int	Server::kick_Command(std::vector<String> arguments, Client &client) 
{
	String cmd = arguments.at(0);

	if (arguments.size() < 3)
	{
		client.reply("461 " + client.get_Nick_name() + " " + cmd + " :Not enough parameters");
		return -1;
	}
	if (this_Channel(arguments.at(1)) == false)
	{
		client.reply("403 " + client.get_Nick_name() + " " + arguments.at(1) + " :No such channel");
		return -1;
	}
	if (isClientInChannel(findChannel(arguments.at(1)), client.get_file_descriptor()) == false)
	{
		client.reply("442 " + client.get_Nick_name() + " " + arguments.at(1) + " :You're not on that channel");
		return -1;
	}
	if (client.get_file_descriptor() != findChannel(arguments.at(1)).get_file_descriptor_with_zero_permissions())
	{
		client.reply(ERR_CHANOPRIVSNEEDED(client, arguments.at(1)));
		return -1;
	}
	if (isClientNInChannel(findChannel(arguments.at(1)), carriage_return(arguments.at(2))) == false)
	{
		client.reply("441 " + client.get_Nick_name() + " " + carriage_return(arguments.at(2)) + " " + arguments.at(1) + " :They aren't on that channel");
		return -1;
	}
	std::vector<String> temporary;
	temporary.push_back("PART");
	temporary.push_back(arguments.at(1));
	part_Command(temporary, find_Client(arguments.at(2)));
	return 0;
}