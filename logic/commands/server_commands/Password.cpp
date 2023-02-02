/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Password.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/02 14:03:50 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String	ERR_PASSWDMISMATCH(Client &client) 
{
	return ("464 " + client.getNickname() + " :Password incorrect");
}

int		teststring(String test)
{
	const char *tst = test.c_str();
	int		i = 0;
	while (tst[i])
	{
		if (tst[i] == '\r')
		{
			std::cout << "character number : " << i << " Line Break\n";
			return (1);
		}
		i++;
	}
	return 0;
}

int Server::cmdPass(std::vector<String> pass, Client &cl) 
{
	if (pass.size() < 2)
	{
		cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "Password"));
		return -1;
	}
	String mdp = erasebr(pass[1]);
	if (mdp != _password)
	{
		std::cout <<"password[1] : " << "[" << mdp << "]"<< std::endl;
		std::cout << "password : " << "[" << _password << "]" << std::endl;
		teststring(pass[1]);
		cl.reply(ERR_PASSWDMISMATCH(cl));
		return -1;
	}
	cl.setState(LOGIN);
	cl.welcome();
	return 0;
}