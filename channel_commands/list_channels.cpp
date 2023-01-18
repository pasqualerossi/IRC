/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_channels.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:36 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 17:55:54 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// NEAT UP!

int Server::command_List(std::vector<String> args, Client &cl) {
    if (args.size() == 1 || (args.size() == 2 && args.at(1) == "\r"))
    {
        if (_channels.size() == 0)
        {

            cl.reply(":NO SUCHCHANNEL"); // je reply 403???
            return -1;
        }
        for (size_t i = 0; i < _channels.size(); i++)
        {
            std::stringstream ss;
            ss << _channels.at(i).get_Clients().size();
            String size = ss.str();
            cl.reply("322 " + cl.get_Nick_name() + " " + _channels.at(i).get_Name() + " " + size + " :" + _channels.at(i).get_Topic());
        }
		cl.reply("323 " + cl.get_Nick_name() + " :End of /LIST");
        return 0;
    }
	std::vector<String> channel_name = split(args.at(1), ",");
    if (args.size() > 1)
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
                    cl.reply("322 " + cl.get_Nick_name() + " " + _channels.at(j).get_Name() + " " + size + " :" + _channels.at(j).get_Topic());
					check = 1;
					break ;

                }
            }
            if (check == 0)
            {
                cl.reply("403 " + cl.get_Nick_name() + " " + carriage_return(channel_name.at(i)) + " :No such channel");
                return -1;
            }
        }
    }
    return 0;
}