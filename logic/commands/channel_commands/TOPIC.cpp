#include "../../headers/Server.hpp"

String  RPL_NOTOPIC(Client cl, String channel) {
	return ("331 " + cl.getNickname() + " " + channel +" :No topic is set");
}

String  ERR_NOTONCHANNEL(Client cl, String channel) {
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
    topic = erasebr(topic); //enleve le \r
    return topic;
}

int		Server::cmdTopic(std::vector<String> args, Client &cl)
{
	if (args.size() < 2)
	{
		cl.reply(ERR_NEEDMOREPARAMS(cl, "TOPIC"));
		return -1;
	}

	String chan_name = erasebr(args[1]);
	if (chan_name.empty())
	{
		cl.reply(ERR_NEEDMOREPARAMS(cl, "TOPIC"));
		return (-1);
	}

	try {
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
			else if (isOperInChannel(cl, *chan))
			{
				chan->setTopic(getTopicStr(args));
				chan->broadcast(RPL_TOPIC(cl, chan_name, chan->getTopic()));
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