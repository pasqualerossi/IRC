#include "../../irc.hpp"

bool     Server::isChannel(std::string name)
{
	for (unsigned int i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getName() == name)
			return true;
	}
	return false;
}

int	Server::cmdKick(std::vector<String> args, Client &cl) 
{
	String cmd = args.at(0);
	if (args.size() < 3)
	{
		cl.reply("461 " + cl.getNickname() + " " + cmd + " :Not enough parameters");
		return -1;
	}
	if (isChannel(args.at(1)) == false)
	{
		cl.reply("403 " + cl.getNickname() + " " + args.at(1) + " :No such channel");
		return -1;
	}
	if (isClientInChannel(findChannel(args.at(1)), cl.getFd()) == false)
	{
		cl.reply("442 " + cl.getNickname() + " " + args.at(1) + " :You're not on that channel");
		return -1;
	}
	if (cl.getFd() != findChannel(args.at(1)).getFdOp())
	{
		cl.reply(ERR_CHANOPRIVSNEEDED(cl, args.at(1)));
		return -1;
	}
	if (isClientNInChannel(findChannel(args.at(1)), erasebr(args.at(2))) == false)
	{
		cl.reply("441 " + cl.getNickname() + " " + erasebr(args.at(2)) + " " + args.at(1) + " :They aren't on that channel");
		return -1;
	}
	std::vector<String> tmp;
	tmp.push_back("PART");
	tmp.push_back(args.at(1));
	cmdPart(tmp, findClient(args.at(2)));
	return 0;
}
