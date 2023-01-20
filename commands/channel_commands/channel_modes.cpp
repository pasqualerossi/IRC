/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_modes.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:07 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 18:32:09 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

// NEAT UP!

int	give_zero_permissions(Client &client, std::vector<String> arguments, Channel &channel) 
{
	int check = 0;
	size_t i = 0;

	std::cout << "HEEEEEEEEEERE" << std::endl;
	if (arguments.size() < 4) 
	{
		client.reply(error_need_more_Parameters(client, "MODE"));
		return -1;
	}
	for (; i < channel.get_Clients().size(); i++)
	{
		if (channel.get_Clients().at(i).get_Nick_name() == carriage_return(arguments.at(3)))
		{
			std::cout << "I'm here" << std::endl;
			check = 1;
			break ;
		}
	}
	if (check == 1)
		channel.set_file_descriptor_to_zero_permissions(channel.get_Clients().at(i).get_file_descriptor());
	else
		client.reply("401 " + client.get_Nick_name() + " " + carriage_return(arguments.at(3)) + " :No such nickname or channel");
	return 0;
}


int set_Password(String password, Channel &channel) 
{
	if (password.empty())
	{
		return -1;
	}
	channel.set_Password(password);
	return 0;
}

int remove_Password(String password, Channel &channel) 
{
	if (password.empty())
	{
		return -1;
	}
	if (channel.get_Password() == password)
	{
		channel.set_Password("");
	}
	else
	{
		std::cout << "Wrong password" << std::endl;
	}
	return 0;
}


size_t	parse_Limit(std::string arguments) 
{
	char	*buffer;
	
	std::string temporary = carriage_return(arguments);
	if (temporary.empty())
	{
		return (0);
	}
	long int limit = strtol(temporary.c_str(), &buffer, 10);
	if (*buffer != '\0')
	{
		return (0);
	}
	return (limit);
}

int	set_Limit(size_t limit, Channel &channel)
{
	if (limit < channel.get_Clients().size() && limit != 0)
	{
		std::cout << "to many people on this channel to set this limit" << std::endl;
		return -1;
	}
	channel.set_Limit(limit);
	return 0;
}

int	check_flag(std::vector<String> arguments, Client &client, Channel &channel) 
{
	int i = 0;
	std::string flags[7] = {"+O","+o","-o","+l","-l","+k","-k"};

	if (arguments.size() < 3)	
	{
		client.reply(error_need_more_Parameters(client, "MODE"));
		return -1;
	}
	String flag = carriage_return(arguments[2]);
	while (flag != flags[i] && i < 7)
	{
		++i;
	}
	std::cout << "IN CHECK_FLAG" << std::endl;
	switch (i) 
	{
		case 0:
			return (give_zero_permissions(client, arguments, channel));
		case 1:
			return (give_zero_permissions(client, arguments, channel));
		case 2:
			client.reply("482 " + client.get_Nick_name() + " " + channel.get_Name() + " :We need an operator");
			return -1;
		case 3:
			if (arguments.size() < 4) 
			{
				client.reply(error_need_more_Parameters(client, "MODE"));
				return -1;
			}
			return (set_Limit(parse_Limit(arguments[3]), channel));
		case 4:
			return (set_Limit(0, channel));
		case 5:
			if (arguments.size() < 4) 
			{
				client.reply(error_need_more_Parameters(client, "MODE"));
				return -1;
			}
			return (set_Password(carriage_return(arguments[3]), channel));
		case 6:
			if (arguments.size() < 4) 
			{
				client.reply(error_need_more_Parameters(client, "MODE"));
				return -1;
			}
			return (remove_Password(carriage_return(arguments[3]), channel));
		default:
			client.reply("501 " + client.get_Nick_name() + " :Unknown MODE flag");
			return -1;
	}
	return 0;
}

int Server::mode_Command(std::vector<String> arguments, Client &client) 
{
	std::cout << "in MODE->args = " << std::endl;
	for (unsigned int i = 0; i < arguments.size(); i++)
		std::cout << arguments[i] << std::endl ;
	std::cout << std::endl;

	if (arguments.size() < 2)
	{
		client.reply(error_need_more_Parameters(client, "MODE"));
		return -1;
	}
	if (this_Channel(arguments.at(1)) == false)
	{
		if (carriage_return(arguments[1]).at(0) != '#')
		{
			return -1;
		}
		client.reply(ERR_NOSUCHCHANNEL(client, carriage_return(arguments[1])));
		return -1;
	}
	if (client.get_file_descriptor() != findChannel(arguments.at(1)).get_file_descriptor_with_zero_permissions())
	{
		client.reply(ERR_CHANOPRIVSNEEDED(client, arguments.at(1)));
		return -1;
	}
	check_flag(arguments, client, findChannel(arguments.at(1)));
	return 1;
}