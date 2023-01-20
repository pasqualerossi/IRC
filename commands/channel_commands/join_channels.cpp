/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_channels.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 16:29:11 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:38:46 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

// NEAT UP!

std::string     RPL_NAMREPLY(Client &cl, std::string chan_name, std::string users)
{
    std::string ret = "353 " + cl.get_Nick_name() + " = " + chan_name + " :" + users;
    std::cout << "namereply " << ret << std::endl;
    return (ret);
}

std::string     RPL_JOIN(Client cl, Channel channel, String topic) {
    if (channel.get_file_descriptor_with_zero_permissions() == cl.get_file_descriptor())
        return (("332 @" + cl.get_Nick_name() + " " + channel.get_Name() + " " + topic));
	else
        return ("332 " + cl.get_Nick_name() + " " + channel.get_Name() + " " + topic);
}

std::string     RPL_ENDOFNAMES(Client &cl, std::string chan_name) {
    return ("366 " + cl.get_Nick_name() + " " + chan_name + " :End of /NAMES list.");
}

void        join(Channel &chan, Client &cl)
{
    std::string users;
    for(unsigned int i = 0; i < chan.get_Clients().size(); i++)
    {
        if (chan.get_Clients()[i].get_file_descriptor() == chan.get_file_descriptor_with_zero_permissions())
            users += "@" + chan.get_Clients()[i].get_Nick_name() + " ";
        else
            users += chan.get_Clients()[i].get_Nick_name() + " ";
    }
    chan.broadcast_message(cl.getPrefix() + " JOIN :" + chan.get_Name());
    
    cl.reply(RPL_TOPIC(cl, chan.get_Name(), chan.get_Topic()));
	cl.reply(RPL_NAMREPLY(cl, chan.get_Name(), users));
	cl.reply(RPL_ENDOFNAMES(cl, chan.get_Name()));
}

int         Server::join_Command(std::vector<String> parameters, Client &client)
{
    if (cl.get_State() != REGISTERED)
    {
        cl.reply("you need to finalize your registration first");
        return -1;
    }
    if (params.size() < 2)
    {
        cl.reply(error_need_more_Parameters(cl, "JOIN"));
        return -1;
    }
    String name = carriage_return(params[1]);
    std::cout << "name = " << name<< std::endl;
    if (!name[0] || name[0] != '#')
    {
        cl.reply("Error : Channel must start with '#'");
        return -1;
    }
    try
    {
        std::vector<Channel>::iterator chan = find_a_Channel(name);
        if (isClientInChannel(*chan, cl.get_file_descriptor()))
        {
            std::cout << "is already in channel" << std::endl;
            return -1;
        }
		if (findChannel(name).get_Clients().size() >= findChannel(name).get_Limit() && findChannel(name).get_Limit() != 0)
		{
			cl.reply("471 " + cl.get_Nick_name() + " " + name + " :Cannot join channel(full)");
			return -1;
		}
		if (findChannel(name).get_Password() != "" && params.size() == 3)
		{
			if (carriage_return(params.at(2)) != findChannel(name).get_Password())
			{
				cl.reply("bad pass");
				return -1;
			}
		}
		if (findChannel(name).get_Password() != "" && params.size() < 3)
		{
			cl.reply("475 " + cl.get_Nick_name() + " " + name + " :bad channel mask");
			return -1;
		}
        chan->add_Client(cl);
        join(*chan, cl);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << "New chan open : " << name << std::endl;
        Channel new_chan(name);
        new_chan.add_Client(cl);
        new_chan.set_file_descriptor_to_zero_permissions(cl.get_file_descriptor());
        _channels.push_back(new_chan);
        join(new_chan, cl);
    }
    _channels[0].debug_channel();
    return 0;
}