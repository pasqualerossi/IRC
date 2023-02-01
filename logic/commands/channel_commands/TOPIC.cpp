/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/01 12:38:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String  RPL_NOTOPIC(Client cl, String channel) 
{
	return ("331 " + cl.getNickname() + " " + channel +" :No topic is set");
}

String  ERR_NOTONCHANNEL(Client cl, String channel) 
{
	return ("442 " + cl.getNickname() + " " + channel + " :You're not on that channel");
}

String      getTopicStr(std::vector<String> params)
{
    String topic;

    unsigned int i = 3;
    topic = params[2];
    if (topic[0] == ':')
        topic = topic.substr(1);
    while (i < params.size())
    {
        topic += " ";
        topic += params[i];
        i++;
    }
    topic = erasebr(topic);
    return topic;
}

int		Server::cmdTopic(std::vector<String> args, Client &cl)
{
	if (args.size() < 2)
	{
		cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "TOPIC"));
		return -1;
	}

	String chan_name = erasebr(args[1]);
	if (chan_name.empty())
	{
		cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "TOPIC"));
		return (-1);
	}
	try 
	{
		std::vector<Channel>::iterator chan = findChannelIt(chan_name);
		if (isClientInChannel(*chan, cl.getFd()))
		{
			if (args.size() == 2)
			{
				if (chan->getTopic().empty())
					cl.reply(RPL_NOTOPIC(cl, chan_name));
				else
					cl.reply(RPL_TOPIC(cl, chan_name, chan->getTopic()));
				return (0);
			}
			else if (is_operator_in_Channel(cl, *chan))
			{
				chan->setTopic(getTopicStr(args));
				chan->broadcast(RPL_TOPIC(cl, chan_name, chan->getTopic()));
				return (0);
			}
			else
				cl.reply(ERROR_CHANNEL_OPERATOR_NEEDED(cl, chan_name));
		}
		else
			cl.reply(ERR_NOTONCHANNEL(cl, chan_name));
	}
	catch (const std::exception& e) 
	{
		cl.reply(ERROR_NO_SUCH_CHANNEL_EXISTS(cl, chan_name));
	}
	return (-1);
}