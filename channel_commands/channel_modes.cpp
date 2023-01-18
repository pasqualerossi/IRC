/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_modes.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:07 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 19:12:44 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// NEAT UP!

int	giveOprivilege(Client &cl, std::vector<String> args, Channel &chan) {
	int check = 0;
	size_t i = 0;
	std::cout << "HEEEEEEEEEERE" << std::endl;
	if (args.size() < 4) {
		cl.reply(error_need_more_Parameters(cl, "MODE"));
		return -1;
	}
	for (; i < chan.get_Clients().size(); i++)
	{
		if (chan.get_Clients().at(i).get_Nick_name() == carriage_return(args.at(3)))
		{
			std::cout << "je suis ici" << std::endl;
			check = 1;
			break ;
		}
	}
	if (check == 1)
		chan.set_file_descriptor_to_zero_permissions(chan.get_Clients().at(i).get_file_descriptor());
	else
		cl.reply("401 " + cl.get_Nick_name() + " " + carriage_return(args.at(3)) + " :No such nick/channel");
	return 0;
}


int set_Password(String password, Channel &chan) {
	if (password.empty())
		return -1;
	chan.set_Password(password);
	return 0;
}

int removePassword(String password, Channel &chan) {
	if (password.empty())
		return -1;

	if (chan.get_Password() == password)
		chan.set_Password("");
	else
		std::cout << "Wrong pass" << std::endl;
	return 0;
}


size_t	parseLimit(std::string arg) {
	char	*buff;

	std::string tmp = carriage_return(arg);
	if (tmp.empty())
		return (0);

	long int limit = strtol(tmp.c_str(), &buff, 10);

	if (*buff != '\0')
		return (0);
	return (limit);
}

int	set_Limit(size_t limit, Channel &chan)
{
	if (limit < chan.get_Clients().size() && limit != 0)
	{
		std::cout << "to many people on chan to set this limit" << std::endl;
		return -1;
	}
	chan.set_Limit(limit);
	return 0;
}

int	check_flag(std::vector<String> args, Client &cl, Channel &chan) {
	int i = 0;
	std::string flags[7] = {"+O","+o","-o","+l","-l","+k","-k"};

	if (args.size() < 3)	{
		cl.reply(error_need_more_Parameters(cl, "MODE"));
		return -1;
	}
	String flag = carriage_return(args[2]);
	while (flag != flags[i] && i < 7)
		++i;
	std::cout << "IN CHECK_FLAG" << std::endl;
	switch (i) {
		case 0:
			return(giveOprivilege(cl, args, chan));
		case 1:
			return (giveOprivilege(cl, args, chan));
		case 2:
			cl.reply("482 " + cl.get_Nick_name() + " " + chan.get_Name() + " :We need an operator");
			return -1;
		case 3:
			if (args.size() < 4) {
				cl.reply(error_need_more_Parameters(cl, "MODE"));
				return -1;
			}
			return (set_Limit(parseLimit(args[3]), chan));
		case 4:
			return (set_Limit(0, chan));
		case 5:
			if (args.size() < 4) {
				cl.reply(error_need_more_Parameters(cl, "MODE"));
				return -1;
			}
			return (set_Password(carriage_return(args[3]), chan));
		case 6:
			if (args.size() < 4) {
				cl.reply(error_need_more_Parameters(cl, "MODE"));
				return -1;
			}
			return (removePassword(carriage_return(args[3]), chan));
		default:
			cl.reply("501 " + cl.get_Nick_name() + " :Unknown MODE flag");
			return -1;
	}
	return 0;
}

int Server::mode_Command(std::vector<String> args, Client &cl) {
	std::cout << "in MODE->args = " << std::endl;
	for (unsigned int i = 0; i < args.size(); i++)
		std::cout << args[i] << std::endl ;
	std::cout << std::endl;

	if (args.size() < 2) //pas sur suivant les cmd qu'on implemente
	{
		cl.reply(error_need_more_Parameters(cl, "MODE"));
		return -1;
	}
	if (this_Channel(args.at(1)) == false)
	{
		if (carriage_return(args[1]).at(0) != '#')
			return -1;
		cl.reply(ERR_NOSUCHCHANNEL(cl, carriage_return(args[1])));
		return -1;
	}
	if (cl.get_file_descriptor() != findChannel(args.at(1)).get_file_descriptorOp())
	{
		cl.reply(ERR_CHANOPRIVSNEEDED(cl, args.at(1)));
		return -1;
	}
	check_flag(args, cl, findChannel(args.at(1)));
	return 1;
}