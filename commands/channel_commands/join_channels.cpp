/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_channels.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:29:11 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

std::string     rpl_name_reply_to_message(Client &client_side, std::string channel_name, std::string users)
{
    std::string ret = "353 " + client_side.get_Nick_name() + " = " + channel_name + " :" + users;
    std::cout << "namereply_to_message " << ret << std::endl;
    return (ret);
}

std::string     RPL_JOIN(Client client_side, Channel channel_side, String topic) 
{
    if (channel_side.getFd_with_zero_permissions() == client_side.getFd())
	{
        return (("332 @" + client_side.get_Nick_name() + " " + channel_side.get_Name() + " " + topic));
	}
	else
	{
        return ("332 " + client_side.get_Nick_name() + " " + channel_side.get_Name() + " " + topic);
	}
}

std::string     rpl_end_of_Names(Client &client_side, std::string channel_name) 
{
    return ("366 " + client_side.get_Nick_name() + " " + channel_name + " :End of /NAMES list.");
}

void        join(Channel &channel_side, Client &client_side)
{
    std::string users;

    for (unsigned int i = 0; i < channel_side.get_Clients().size(); i++)
    {
        if (channel_side.get_Clients()[i].getFd() == channel_side.getFd_with_zero_permissions())
		{
            users += "@" + channel_side.get_Clients()[i].get_Nick_name() + " ";
		}
        else
		{
            users += channel_side.get_Clients()[i].get_Nick_name() + " ";
		}
    }
    channel_side.broadcast_message(client_side.getPrefix() + " JOIN :" + channel_side.get_Name());
    
    client_side.reply_to_message(rpl_topic_Message(client_side, channel_side.get_Name(), channel_side.get_Topic()));
	client_side.reply_to_message(rpl_name_reply_to_message(client_side, channel_side.get_Name(), users));
	client_side.reply_to_message(rpl_end_of_Names(client_side, channel_side.get_Name()));
}

int         Server::join_Command(std::vector<String> parameters, Client &client_side)
{
    if (client_side.get_State() != REGISTERED)
    {
        client_side.reply_to_message("you need to finalize your registration first");
        return -1;
    }
    if (parameters.size() < 2)
    {
        client_side.reply_to_message(error_need_more_Parameters(client_side, "JOIN"));
        return -1;
    }
    String name = carriage_return(parameters[1]);
    std::cout << "name = " << name<< std::endl;
    if (!name[0] || name[0] != '#')
    {
        client_side.reply_to_message("Error : Channel must start with '#'");
        return -1;
    }
    try
    {
        std::vector<Channel>::iterator channel = find_a_Channel(name);
        if (is_client_in_Channel(*chan, client_side.getFd()))
        {
            std::cout << "is already in channel" << std::endl;
            return -1;
        }
		if (find_Channel(name).get_Clients().size() >= find_Channel(name).get_Limit() && find_Channel(name).get_Limit() != 0)
		{
			client_side.reply_to_message("471 " + client_side.get_Nick_name() + " " + name + " :Cannot join channel(full)");
			return -1;
		}
		if (find_Channel(name).get_Password() != "" && parameters.size() == 3)
		{
			if (carriage_return(parameters.at(2)) != find_Channel(name).get_Password())
			{
				client_side.reply_to_message("bad pass");
				return -1;
			}
		}
		if (find_Channel(name).get_Password() != "" && parameters.size() < 3)
		{
			client_side.reply_to_message("475 " + client_side.get_Nick_name() + " " + name + " :bad channel mask");
			return -1;
		}
        chan->add_Client(client_side);
        join(*channel, client_side);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << "New channel opened : " << name << std::endl;
        Channel new_channel(name);
        new_channel.add_Client(client);
        new_channel.set_file_descriptor_to_zero_permissions(client_side.getFd());
        _channels.push_back(new_channel);
        join(new_channel, client);
    }
    _channels[0].debug_channel();
    return 0;
}