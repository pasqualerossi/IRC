/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ping.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:49 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::ping_command(std::vector<String> arguments, Client &client_side)
{
    if (arguments.size() < 2)
	{
		client_side.reply_to_message(error_need_more_Parameters(client_side, "PING"));
		return (-1);
	}
    client_side.reply_to_message("PONG " + arguments[1]);
    return (0);
}