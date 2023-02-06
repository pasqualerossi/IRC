/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/02 14:12:44 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String  NO_SET_TOPIC(Client cl, String channel) 
{
	return ("331 " + cl.getNickname() + " " + channel +" :No topic is set");
}

String  ERROR_NOT_ON_CHANNEL(Client cl, String channel) 
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
		cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "Topic"));
		return -1;
	}
	String chan_name = erasebr(args[1]);
	if (chan_name.empty())
	{
		cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "Topic"));
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
					cl.reply(NO_SET_TOPIC(cl, chan_name));
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
			cl.reply(ERROR_NOT_ON_CHANNEL(cl, chan_name));
	}
	catch (const std::exception& e) 
	{
		cl.reply(ERROR_NO_SUCH_CHANNEL_EXISTS(cl, chan_name));
	}
	return (-1);
}