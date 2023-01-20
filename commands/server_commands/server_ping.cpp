/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ping.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:49 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:39:17 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::ping_command(std::vector<String> arguments, Client &client)
{
    if (arguments.size() < 2)
	{
		client.reply(error_need_more_Parameters(client, "PING"));
		return (-1);
	}
    client.reply("PONG " + arguments[1]);
    return (0);
}