/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/02 14:23:51 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

std::string     RPL_NAMREPLY(Client &cl, std::string chan_name, std::string users)
{
    std::string ret = "353 " + cl.getNickname() + " = " + chan_name + " :" + users;
    std::cout << "name reply " << ret << std::endl;
    return (ret);
}

std::string     RPL_JOIN(Client cl, Channel channel, String topic) 
{
    if (channel.getFdOp() == cl.getFd())
        return (("332 @" + cl.getNickname() + " " + channel.getName() + " " + topic));
	else
        return ("332 " + cl.getNickname() + " " + channel.getName() + " " + topic);
}

std::string     RPL_ENDOFNAMES(Client &cl, std::string chan_name) 
{
    return ("366 " + cl.getNickname() + " " + chan_name + " :End of /Names list.");
}

void        join(Channel &chan, Client &cl)
{
    std::string users;
    for(unsigned int i = 0; i < chan.getClients().size(); i++)
    {
        if (chan.getClients()[i].getFd() == chan.getFdOp())
            users += "@" + chan.getClients()[i].getNickname() + " ";
        else
            users += chan.getClients()[i].getNickname() + " ";
    }
    chan.broadcast(cl.getPrefix() + " Join :" + chan.getName());
    
    cl.reply(RPL_TOPIC(cl, chan.getName(), chan.getTopic()));
	cl.reply(RPL_NAMREPLY(cl, chan.getName(), users));
	cl.reply(RPL_ENDOFNAMES(cl, chan.getName()));
}

int         Server::cmdJoin(std::vector<String> params, Client &cl)
{
    if (cl.getState() != REGISTERED)
    {
        cl.reply("you need to finalise your registration first");
        return -1;
    }
    if (params.size() < 2)
    {
        cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "Join"));
        return -1;
    }
    String name = erasebr(params[1]);
    std::cout << "name = " << name<< std::endl;
    if (!name[0] || name[0] != '#')
    {
        cl.reply("Error : Channel must start with '#'");
        return -1;
    }
    try
    {
        std::vector<Channel>::iterator chan = findChannelIt(name);
        if (isClientInChannel(*chan, cl.getFd()))
        {
            std::cout << "is already in channel" << std::endl;
            return -1;
        }
		if (findChannel(name).getClients().size() >= findChannel(name).getLimit() && findChannel(name).getLimit() != 0)
		{
			cl.reply("471 " + cl.getNickname() + " " + name + " :Cannot join channel (full)");
			return -1;
		}
		if (findChannel(name).getPassword() != "" && params.size() == 3)
		{
			if (erasebr(params.at(2)) != findChannel(name).getPassword())
			{
				cl.reply("bad password");
				return -1;
			}
		}
		if (findChannel(name).getPassword() != "" && params.size() < 3)
		{
			cl.reply("475 " + cl.getNickname() + " " + name + " :bad channel mask");
			return -1;
		}
        chan->addClient(cl);
        join(*chan, cl);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        std::cout << "New channel opened : " << name << std::endl;
        Channel new_chan(name);
        new_chan.addClient(cl);
        new_chan.setFdOp(cl.getFd());
        _channels.push_back(new_chan);
        join(new_chan, cl);
    }
    _channels[0].debug();
    return 0;
}