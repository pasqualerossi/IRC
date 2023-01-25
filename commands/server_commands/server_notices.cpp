/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_notices.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:21 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String rpl_Notice(Client client_side, String recipient, String message)
{
    return (":" + client_side.get_Nick_name() + " NOTICE " + recipient + " :" + message);
}

String      get_message_Notice(std::vector<String> parameters)
{
    String message = parameters[2];
	unsigned int i = 3;
    if (message[0] == ':')
	{
        message = message.substr(1);
	}
    while (i < parameters.size())
    {
        message += " ";
        message += parameters[i];
        i++;
    }
    message = carriage_return(message);
    return message;
}

int Server::change_Notice(std::vector<String> parameters, Client &client_side)
{
    String  message = get_message_Notice(parameters);
	if (parameters.size() < 3)
    {
        client_side.reply_to_message("need more parameters");
        return -1;
    }
    try
    {
        std::vector<Channel>::iterator change = find_a_Channel(parameters[1]);
        if (is_client_in_Channel(*change, client_side.getFd()))
		{
            change->broadcast_message(rpl_Notice(client_side, parameters[1], message), client_side);
		}
		else
		{
            client_side.reply_to_message("404 " + client_side.get_Nick_name() + " you are not in the channel " + change->get_Name());
		}
    }
    catch(const std::exception& e)
    {
       client_side.reply_to_message(e.what());
    }
    return 0;
}

int Server::notice_Command(std::vector<String> parameters, Client &client_side)
{
    std::cout << "ENTER IN NOTICE" << std::endl;
    if (client_side.get_State() != REGISTERED)
    {
        client_side.reply_to_message("you need to register first (prvmsg)");
        return -1;
    }
    if (parameters.size() < 3)
	{
        std::cout << "paramas size = "<< parameters.size() << std::endl;
        client_side.reply_to_message("461 " + client_side.get_Nick_name() + " " + "NOTICE" + " :Not enough parameters");
		return -1;
	}
    if (parameters[1].at(0) == '#')
        return (change_Notice(parameters, client_side));
    try
    {
        Client  recipient = find_Client(parameters[1]);      
        String  message = get_message_Notice(parameters);
        String packet = rpl_Notice(client_side, recipient.get_Nick_name(), message);

        std::cout << packet << recipient.getFd() << std::endl;
        packet += "\r\n";
        if (send(recipient.getFd(), packet.c_str(), packet.length(), 0) < 0)
		{
            throw std::out_of_range("error while sendig in notice");
		}
    }
    catch(const std::exception& e)
    {
        client_side.reply_to_message(e.what());
        std::cerr << e.what() << '\n';
    }
    return 0;
}