/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic_channels.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:52 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 19:12:44 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// NEAT UP!

String  RPL_NOTOPIC(Client cl, String channel) {
	return ("331 " + cl.get_Nick_name() + " " + channel +" :No topic is set");
}

String  ERR_NOTONCHANNEL(Client cl, String channel) {
	return ("442 " + cl.get_Nick_name() + " " + channel + " :You're not on that channel");
}

String      get_TopicStr(std::vector<String> params)
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
    topic = carriage_return(topic); //enleve le \r
    return topic;
}

int		Server::topic_command(std::vector<String> args, Client &cl)
{
	if (args.size() < 2)
	{
		cl.reply(error_need_more_Parameters(cl, "TOPIC"));
		return -1;
	}

	String chan_name = carriage_return(args[1]);
	if (chan_name.empty())
	{
		cl.reply(error_need_more_Parameters(cl, "TOPIC"));
		return (-1);
	}

	try {
		std::vector<Channel>::iterator chan = find_a_Channel(chan_name);

		if (isClientInChannel(*chan, cl.get_file_descriptor()))
		{
			if (args.size() == 2)
			{
				if (chan->get_Topic().empty())
					cl.reply(RPL_NOTOPIC(cl, chan_name));
				else
					cl.reply(RPL_TOPIC(cl, chan_name, chan->get_Topic()));
				return (0);
			}
			else if (isOperInChannel(cl, *chan))
			{
				chan->set_Topic(get_TopicStr(args));
				chan->broadcast_message(RPL_TOPIC(cl, chan_name, chan->get_Topic()));
				return (0);
			}
			else
				cl.reply(ERR_CHANOPRIVSNEEDED(cl, chan_name));
		}
		else
			cl.reply(ERR_NOTONCHANNEL(cl, chan_name));
	}
	catch (const std::exception& e) {
		cl.reply(ERR_NOSUCHCHANNEL(cl, chan_name));
	}
	return (-1);

}