#include "../../headers/Server.hpp"

int	Server::cmdNames(std::vector<String> args, Client &cl) {
	
	if (args.size() == 1)
	{
		std::vector<String> tmp;
		tmp.push_back("LIST");
		cmdList(tmp, cl);
		return 0;
	}
	std::vector<String> channel_name = split(args.at(1), ",");
	if (args.size() > 1)
	{
		size_t i = 0;
		for (; i < channel_name.size(); i++)
		{
			for (size_t j = 0; j < _channels.size(); j++)
			{
				if (erasebr(channel_name.at(i)) == _channels.at(j).getName())
				{
					String str;
					size_t x = 0;
					for (; x < _channels.at(j).getClients().size() - 1; x++)
					{
						if (cl.getFd() == _channels.at(i).getFdOp())
							str += "@" + _channels.at(j).getClients().at(x).getNickname() + " ";
						else
							str += _channels.at(j).getClients().at(x).getNickname() + " ";
					}
					str += _channels.at(j).getClients().at(x).getNickname();
					cl.reply("353 " + cl.getNickname() + " = " + _channels.at(j).getName() + " :" + str);
					str.clear();
					cl.reply("366 " + cl.getNickname() + " " + _channels.at(j).getName() + " :End of /NAMES list");
					
				}
			}
			
		}
		
		
	}
	return 0;
}