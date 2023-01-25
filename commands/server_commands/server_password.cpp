/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_password.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:37 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String	error_not_matching_Password(Client &client_side) 
{
	return ("464 " + client_side.get_Nick_name() + " - Password is incorrect");
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

int Server::password_Command(std::vector<String> password, Client &client_side) 
{
	String multiple_passwords = carriage_return(password[1]);
	if (password.size() < 2)
	{
		client_side.reply_to_message(error_need_more_Parameters(client_side, "PASSWORD"));
		return -1;
	}
	if (multiple_passwords != _password)
	{
		std::cout <<"password[1] - " << "[" << multiple_passwords << "]"<< std::endl;
		std::cout << "password - " << "[" << _password << "]" << std::endl;
		testing_the_string(password[1]);
		client_side.reply_to_message(error_not_matching_Password(client_side));
		return -1;
	}
	client_side.set_State(LOGIN);
	client_side.welcome_message();
	return 0;
}