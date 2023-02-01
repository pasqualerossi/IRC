/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/01 12:44:24 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::cmdList(std::vector<String> args, Client &cl) 
{
    if (args.size() == 1 || (args.size() == 2 && args.at(1) == "\r"))
    {
        if (_channels.size() == 0)
        {
            cl.reply(":NO SUCHCHANNEL");
            return -1;
        }
        for (size_t i = 0; i < _channels.size(); i++)
        {
            std::stringstream ss;
            ss << _channels.at(i).getClients().size();
            String size = ss.str();
            cl.reply("322 " + cl.getNickname() + " " + _channels.at(i).getName() + " " + size + " :" + _channels.at(i).getTopic());
        }
		cl.reply("323 " + cl.getNickname() + " :End of /LIST");
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
                if (erasebr(channel_name.at(i)) == _channels.at(j).getName())
                {
                    std::stringstream ss;
                    ss << _channels.at(j).getClients().size();
                    String size = ss.str();
                    cl.reply("322 " + cl.getNickname() + " " + _channels.at(j).getName() + " " + size + " :" + _channels.at(j).getTopic());
					check = 1;
					break ;
                }
            }
            if (check == 0)
            {
                cl.reply("403 " + cl.getNickname() + " " + erasebr(channel_name.at(i)) + " :No such channel");
                return -1;
            }
        }
    }
    return 0;
}
