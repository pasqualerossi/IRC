#include "irc.hpp"

String	erasebr(String str) 
{
	if (str.empty())
		return "";
	if (str[str.size() - 1] == '\r')
		return str.substr(0, (str.size() - 1));
	return str;
}

String	ERR_NEEDMOREPARAMS(Client &client, String cmd) 
{
	return ("461 " + client.getNickname() + " " + cmd + " :Not enough parameters");
}

String  ERR_NOSUCHCHANNEL(Client cl, String channel) 
{
	return ("403 " + cl.getNickname() + " " + channel +" :No such channel");
}

String  ERR_CHANOPRIVSNEEDED(Client cl, String channel) 
{
	return ("482 " + cl.getNickname() + " " + channel + " :You're not channel operator");
}

String  RPL_TOPIC(Client cl, String channel, String topic) 
{
	return ("332 " + cl.getNickname() + " " + channel + " " + topic);
}

bool	isClientInChannel(Channel &chan, int fd)
{
	for (unsigned int i = 0; i < chan.getClients().size(); i++)
	{
		if (chan.getClients()[i].getFd() == fd)
			return true;
	}
	return false;
}

bool	isClientNInChannel(Channel &chan, String name)
{
	for (unsigned int i = 0; i < chan.getClients().size(); i++)
	{
		if (chan.getClients()[i].getNickname() == name)
			return true;
	}
	return false;
}

bool	isOperInChannel(Client cl, Channel chan)
{
	if (chan.getFdOp() == cl.getFd())
		return true;
	return false;
}

std::vector<String> split(String str, const char *delim)
{
	std::vector<String> list;

	char 	*ptr = strtok((char *)str.c_str(), delim);

	while (ptr)
	{
		list.push_back(std::string(ptr));
		ptr = strtok(NULL, delim);
	}
	return list;
}