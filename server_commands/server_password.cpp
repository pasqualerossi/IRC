/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_password.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:37 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 19:12:44 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// NEAT UP!

String	ERR_PASSWDMISMATCH(Client &client) {
	return ("464 " + client.get_Nick_name() + " :Password incorrect");
}

int		teststring(String test)
{
	const char *tst = test.c_str();
	int		i = 0;
	while (tst[i])
	{
		if (tst[i] == '\r')
		{
			std::cout << "charatere numero : " << i << " RETOUR A LA LIGNE\n";
			return (1);
		}
		i++;
	}
	return 0;
}

int Server::password_Command(std::vector<String> pass, Client &cl) {
	if (pass.size() < 2)
	{
		cl.reply(error_need_more_Parameters(cl, "PASS"));
		return -1;
	}
	String mdp = carriage_return(pass[1]); // enleve le \r a la fin de pass
	if (mdp != _password)
	{
		std::cout <<"pass[1] : " << "[" << mdp << "]"<< std::endl;
		std::cout << "password : " << "[" << _password << "]" << std::endl;
		teststring(pass[1]);
		cl.reply(ERR_PASSWDMISMATCH(cl));
		return -1;
	}
	cl.setState(LOGIN);
	cl.welcome_message();
	return 0;
}