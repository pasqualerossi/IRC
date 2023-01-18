/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick_channels.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:30 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 18:36:47 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// NEAT UP!

bool     Server::this_Channel(std::string name)
{
	for (unsigned int i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].get_Name() == name)
			return true;
	}
	return false;
}

int	Server::kick_Command(std::vector<String> args, Client &cl) 
{
	String cmd = args.at(0);
	if (args.size() < 3)
	{
		cl.reply("461 " + cl.get_Nick_name() + " " + cmd + " :Not enough parameters");
		return -1;
	}
	if (this_Channel(args.at(1)) == false)
	{
		cl.reply("403 " + cl.get_Nick_name() + " " + args.at(1) + " :No such channel");
		return -1;
	}
	if (isClientInChannel(findChannel(args.at(1)), cl.get_file_descriptor()) == false)
	{
		cl.reply("442 " + cl.get_Nick_name() + " " + args.at(1) + " :You're not on that channel");
		return -1;
	}
	if (cl.get_file_descriptor() != findChannel(args.at(1)).get_file_descriptorOp())
	{
		cl.reply(ERR_CHANOPRIVSNEEDED(cl, args.at(1)));
		return -1;
	}
	if (isClientNInChannel(findChannel(args.at(1)), carriage_return(args.at(2))) == false)
	{
		cl.reply("441 " + cl.get_Nick_name() + " " + carriage_return(args.at(2)) + " " + args.at(1) + " :They aren't on that channel");
		return -1;
	}
	std::vector<String> tmp;
	tmp.push_back("PART");
	tmp.push_back(args.at(1));
	part_Command(tmp, findClient(args.at(2)));
	return 0;
}