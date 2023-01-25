/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_operator.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:28 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int	Server::operator_command(std::vector<String> arguments, Client &client_side)
{
	String command = arguments.at(0);
	String password = carriage_return(arguments.at(2));
	for (size_t i = 0; i < arguments.size(); i++)
	{
		std::cout << arguments.at(i) << std::endl;
	}
	if (arguments.size() < 3)
	{
		client_side.reply_to_message("461 " + client_side.get_Nick_name() + " " + command + " :Not enough parameters");
		return -1;
	}
	if (this->_operator_password != password)
	{
		client_side.reply_to_message("464 " + client_side.get_Nick_name() + " " + command + " :Password incorrect");
		return -1;
	}
	else
	{
		client_side.set_is_operator(true);
		client_side.reply_to_message("381 " + client_side.get_Nick_name() + " " + command + " :you are now an IRC operator");
	}
	return 0;
}