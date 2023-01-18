/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_names.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:15 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 18:36:47 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

int	Server::channel_Names(std::vector<String> arguments, Client &client) 
{	
	if (arguments.size() == 1)
	{
		std::vector<String> temporary;
		temporary.push_back("LIST");
		command_List(temporary, client);
		return 0;
	}

	std::vector<String> channel_name = split(arguments.at(1), ",");

	if (arguments.size() > 1)
	{
		size_t i = 0;

		for (; i < channel_name.size(); i++)
		{
			for (size_t j = 0; j < _channels.size(); j++)
			{
				if (carriage_return(channel_name.at(i)) == _channels.at(j).get_Name())
				{
					String string;
					size_t x = 0;

					for (; x < _channels.at(j).get_Clients().size() - 1; x++)
					{
						if (client.get_file_descriptor() == _channels.at(i).get_file_descriptorOp())
						{
							string += "@" + _channels.at(j).get_Clients().at(x).get_Nick_name() + " ";
						}
						else
						{
							string += _channels.at(j).get_Clients().at(x).get_Nick_name() + " ";
						}
					}
					string += _channels.at(j).get_Clients().at(x).get_Nick_name();
					client.reply("353 " + client.get_Nick_name() + " = " + _channels.at(j).get_Name() + " :" + string);
					string.clear();
					client.reply("366 " + client.get_Nick_name() + " " + _channels.at(j).get_Name() + " :End of /NAMES list");
				}
			}
		}
	}
	return 0;
}