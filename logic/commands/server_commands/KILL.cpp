#include "../../headers/Server.hpp"

int	Server::cmdKill(std::vector<String> args, Client &cl)
{
	String cmd = args.at(0);
	size_t	i = 0;
	int		check = 0;

	if (cl.getisoper() == false)
	{
		cl.reply("481 " + cl.getNickname() + " " +  cmd + " :Permission Denied- You're not an IRC operator");
		return -1;
	}
	if (args.size() < 2)
	{
		cl.reply("461 " + cl.getNickname() + " " + cmd + " :Not enough parameters");
		return -1;
	}
	for (i = 0; i < _clients.size(); i++)
	{
		if (args.at(1) == _clients.at(i).getNickname())
		{
			check = 1;
			break ;
		}
	}
	if (check != 1)
	{
		cl.reply("401 " + cl.getNickname() + " " + args.at(1) + " :No such nick");
		return -1;
	}
	clientDisconnect(_clients.at(i).getFd());
	return 0;
}