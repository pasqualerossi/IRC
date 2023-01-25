/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic_channels.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:52 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String  rpl_no_Topic(Client client_side, String channel_side) 
{
	return ("331 " + client_side.get_Nick_name() + " " + channel_side +" :No topic is set");
}

String  error_not_on_Channel(Client client_side, String channel_side) 
{
	return ("442 " + client_side.get_Nick_name() + " " + channel_side + " :You're not on that channel");
}

String      get_TopicStr(std::vector<String> parameters)
{
    String topic = parameters[2];
    unsigned int i = 3;
    if (topic[0] == ':')
        topic = topic.substr(1);
    while (i < parameters.size())
    {
        topic += " ";
        topic += parameters[i];
        i++;
    }
    topic = carriage_return(topic);
    return topic;
}

int		Server::topic_command(std::vector<String> arguments, Client &client_side)
{
	String channel_name = carriage_return(arguments[1]);
	std::vector<Channel>::iterator channel = find_a_Channel(channel_name);

	if (arguments.size() < 2)
	{
		client_side.reply_to_message(error_need_more_Parameters(client_side, "TOPIC"));
		return -1;
	}
	if (channel_name.empty())
	{
		client_side.reply_to_message(error_need_more_Parameters(client_side, "TOPIC"));
		return (-1);
	}
	try 
	{
		if (is_client_in_Channel(*channel, client_side.getFd()))
		{
			if (arguments.size() == 2)
			{
				if (channel->get_Topic().empty())
				{
					client_side.reply_to_message(rpl_no_Topic(client_side, channel_name));
				}
				else
				{
					client_side.reply_to_message(rpl_topic_Message(client_side, channel_name, channel->get_Topic()));
				}
				return (0);
			}
			else if (is_operator_in_Channel(client_side, *channel))
			{
				channel->set_Topic(get_TopicStr(args));
				channel->broadcast_message(rpl_topic_Message(client_side, channel_name, channel->get_Topic()));
				return (0);
			}
			else
			{
				client_side.reply_to_message(error_channel_operator_is_Needed(client_side, channel_name));
			}
		}
		else
		{
			client_side.reply_to_message(error_not_on_Channel(client_side, channel_name));
		}
	}
	catch (const std::exception& e) 
	{
		client_side.reply_to_message(error_no_such_Channel(client_side, channel_name));
	}
	return (-1);

}