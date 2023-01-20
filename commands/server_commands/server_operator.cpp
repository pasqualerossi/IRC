/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:28 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:39:22 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int	Server::operator_command(std::vector<String> arguments, Client &client)
{
	String command = arguments.at(0);
	String password = carriage_return(arguments.at(2));
	
	for (size_t i = 0; i < arguments.size(); i++)
	{
		std::cout << arguments.at(i) << std::endl;
	}
	if (arguments.size() < 3)
	{
		client.reply("461 " + client.get_Nick_name() + " " + command + " :Not enough parameters");
		return -1;
	}
	if (this->_operator_password != password)
	{
		client.reply("464 " + client.get_Nick_name() + " " + command + " :Password incorrect");
		return -1;
	}
	else
	{
		client.set_is_operator(true);
		client.reply("381 " + client.get_Nick_name() + " " + command + " :you are now an IRC operator");
	}
	return 0;
}