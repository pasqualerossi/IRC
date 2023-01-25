/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_channel.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:43 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::part_Command(std::vector<String> parameters, Client &client_side)
{
    if (client_side.get_State() != REGISTERED)
	{
        client_side.reply_to_message("u need to register first");
        return -1;
    }
    if (parameters.size() < 2)
    {
        client_side.reply_to_message(error_need_more_Parameters(client_side, "PART"));
        return -1;
    }
    try
    {
        String change_name = carriage_return(parameters[1]);
        std::vector<Channel>::iterator change = find_a_Channel(change_name);

        if (change->getFd_with_zero_permissions() == client_side.getFd())
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
        client_side.reply_to_message("403 " + client_side.get_Nick_name() + " " + parameters.at(1) + " :No such channel");
    }
    return 0;
}