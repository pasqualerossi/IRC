/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/02 14:01:23 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::cmdPing(std::vector<String> args, Client &cl)
{
    if (args.size() < 2)
	{
		cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "Ping"));
		return (-1);
	}
    cl.reply("Pong " + args[1]);
    return (0);
}