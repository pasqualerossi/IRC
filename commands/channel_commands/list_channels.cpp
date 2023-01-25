/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_channels.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:36 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::command_List(std::vector<String> arguments, Client &client_side) 
{
    if (arguments.size() == 1 || (arguments.size() == 2 && arguments.at(1) == "\r"))
    {
        if (_channels.size() == 0)
        {
            client_side.reply_to_message("- NO SUCH CHANNEL");
            return -1;
        }
        for (size_t i = 0; i < _channels.size(); i++)
        {
            std::stringstream ss;
            ss << _channels.at(i).get_Clients().size();
            String size = ss.str();
            client_side.reply_to_message("322 " + client_side.get_Nick_name() + " " + _channels.at(i).get_Name() + " " + size + " :" + _channels.at(i).get_Topic());
        }
		client_side.reply_to_message("323 " + client_side.get_Nick_name() + " :End of /LIST");
        return 0;
    }
	std::vector<String> channel_name = split(arguments.at(1), ",");
    if (arguments.size() > 1)
    {
        for (size_t i = 0; i < channel_name.size(); i++)
        {
			int check = 0;
            size_t j = 0;
            for (; j < _channels.size() ; j++)
            {
                if (carriage_return(channel_name.at(i)) == _channels.at(j).get_Name())
                {
                    std::stringstream ss;
                    ss << _channels.at(j).get_Clients().size();
                    String size = ss.str();
                    client_side.reply_to_message("322 " + client_side.get_Nick_name() + " " + _channels.at(j).get_Name() + " " + size + " :" + _channels.at(j).get_Topic());
					check = 1;
					break ;
                }
            }
            if (check == 0)
            {
                client_side.reply_to_message("403 " + client_side.get_Nick_name() + " " + carriage_return(channel_name.at(i)) + " :No such channel");
                return -1;
            }
        }
    }
    return 0;
}