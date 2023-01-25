/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_modes.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:07 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int	give_zero_permissions(Client &client_side, std::vector<String> arguments, Channel &channel_side) 
{
	int check = 0;
	size_t i = 0;

	std::cout << "HEEEEEEEEEERE" << std::endl;
	if (arguments.size() < 4) 
	{
		client_side.reply_to_message(error_need_more_Parameters(client_side, "MODE"));
		return -1;
	}
	for (; i < channel_side.get_Clients().size(); i++)
	{
		if (channel_side.get_Clients().at(i).get_Nick_name() == carriage_return(arguments.at(3)))
		{
			std::cout << "I'm here" << std::endl;
			check = 1;
			break ;
		}
	}
	if (check == 1)
		channel_side.set_file_descriptor_to_zero_permissions(channel_side.get_Clients().at(i).getFd());
	else
		client_side.reply_to_message("401 " + client_side.get_Nick_name() + " " + carriage_return(arguments.at(3)) + " :No such nickname or channel");
	return 0;
}


int set_Password(String password, Channel &channel_side) 
{
	if (password.empty())
	{
		return -1;
	}
	channel_side.set_Password(password);
	return 0;
}

int remove_Password(String password, Channel &channel_side) 
{
	if (password.empty())
	{
		return -1;
	}
	if (channel_side.get_Password() == password)
	{
		channel_side.set_Password("");
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

int	set_Limit(size_t limit, Channel &channel_side)
{
	if (limit < channel_side.get_Clients().size() && limit != 0)
	{
		std::cout << "to many people on this channel to set this limit" << std::endl;
		return -1;
	}
	channel_side.set_Limit(limit);
	return 0;
}

int	check_flag(std::vector<String> arguments, Client &client_side, Channel &channel_side) 
{
	int i = 0;
	std::string flags[7] = {"+O","+o","-o","+l","-l","+k","-k"};

	if (arguments.size() < 3)	
	{
		client_side.reply_to_message(error_need_more_Parameters(client_side, "MODE"));
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
			return (give_zero_permissions(client_side, arguments, channel_side));
		case 1:
			return (give_zero_permissions(client_side, arguments, channel_side));
		case 2:
			client_side.reply_to_message("482 " + client_side.get_Nick_name() + " " + channel_side.get_Name() + " :We need an operator");
			return -1;
		case 3:
			if (arguments.size() < 4) 
			{
				client_side.reply_to_message(error_need_more_Parameters(client_side, "MODE"));
				return -1;
			}
			return (set_Limit(parse_Limit(arguments[3]), channel_side));
		case 4:
			return (set_Limit(0, channel_side));
		case 5:
			if (arguments.size() < 4) 
			{
				client_side.reply_to_message(error_need_more_Parameters(client_side, "MODE"));
				return -1;
			}
			return (set_Password(carriage_return(arguments[3]), channel_side));
		case 6:
			if (arguments.size() < 4) 
			{
				client_side.reply_to_message(error_need_more_Parameters(client_side, "MODE"));
				return -1;
			}
			return (remove_Password(carriage_return(arguments[3]), channel_side));
		default:
			client_side.reply_to_message("501 " + client_side.get_Nick_name() + " :Unknown MODE flag");
			return -1;
	}
	return 0;
}

int Server::mode_Command(std::vector<String> arguments, Client &client_side) 
{
	std::cout << "in MODE->args = " << std::endl;
	for (unsigned int i = 0; i < arguments.size(); i++)
		std::cout << arguments[i] << std::endl ;
	std::cout << std::endl;

	if (arguments.size() < 2)
	{
		client_side.reply_to_message(error_need_more_Parameters(client_side, "MODE"));
		return -1;
	}
	if (this_Channel(arguments.at(1)) == false)
	{
		if (carriage_return(arguments[1]).at(0) != '#')
		{
			return -1;
		}
		client_side.reply_to_message(error_no_such_Channel(client_side, carriage_return(arguments[1])));
		return -1;
	}
	if (client_side.getFd() != find_Channel(arguments.at(1)).getFd_with_zero_permissions())
	{
		client_side.reply_to_message(error_channel_operator_is_Needed(client_side, arguments.at(1)));
		return -1;
	}
	check_flag(arguments, client_side, find_Channel(arguments.at(1)));
	return 1;
}