/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_password.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:37 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:39:20 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String	error_not_matching_Password(Client &client) {
	return ("464 " + client.get_Nick_name() + " - Password is incorrect");
}

int		testing_the_string(String test)
{
	const char *testing = test.c_str();
	int		i = 0;

	while (testing[i])
	{
		if (testing[i] == '\r')
		{
			std::cout << "Character Number - " << i << " Line Break\n";
			return (1);
		}
		i++;
	}
	return 0;
}

int Server::password_Command(std::vector<String> password, Client &client) 
{
	String multiple_passwords = carriage_return(password[1]);

	if (password.size() < 2)
	{
		client.reply(error_need_more_Parameters(client, "PASSWORD"));
		return -1;
	}
	if (multiple_passwords != _password)
	{
		std::cout <<"password[1] - " << "[" << multiple_passwords << "]"<< std::endl;
		std::cout << "password - " << "[" << _password << "]" << std::endl;
		testing_the_string(password[1]);
		client.reply(error_not_matching_Password(client));
		return -1;
	}
	client.set_State(LOGIN);
	client.welcome_message();
	return 0;
}