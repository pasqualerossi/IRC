#include "../../irc.hpp"

int Server::cmdPing(std::vector<String> args, Client &cl)
{
    if (args.size() < 2)
	{
		cl.reply(ERR_NEEDMOREPARAMS(cl, "PING"));
		return (-1);
	}
    cl.reply("PONG " + args[1]);
    return (0);
}