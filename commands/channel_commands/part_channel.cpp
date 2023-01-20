/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_channel.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:43 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:42:40 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int         Server::part_Command(std::vector<String> parameters, Client &client)
{
    if (client.get_State() != REGISTERED)
	{
        client.reply("u need to register first");
        return -1;
    }
    if (parameters.size() < 2)
    {
        client.reply(error_need_more_Parameters(client, "PART"));
        return -1;
    }
    try
    {
        String change_name = carriage_return(parameters[1]);
        std::vector<Channel>::iterator change = find_a_Channel(change_name);

        if (change->get_file_descriptor_with_zero_permissions() == client.get_file_descriptor())
		{
            change->set_file_descriptor_to_zero_permissions(0);
		}
        change->eraseClient(cl);
        if (change->get_Clients().empty())
		{
            _channels.erase(change);
		}
    }
    catch(const std::exception& e)
    {
        client.reply("403 " + client.get_Nick_name() + " " + parameters.at(1) + " :No such channel");
    }
    return 0;
}