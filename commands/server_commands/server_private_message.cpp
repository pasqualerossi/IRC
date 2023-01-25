/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_private_message.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:57 by prossi            #+#    #+#             */
/*   Updated: 2023/01/23 21:35:31 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String rpl_private_message(Client client_side, String recipient, String message)
{
    return (":" + client_side.get_Nick_name() + " Private Message " + recipient + " :" + message);
}

String      get_Message(std::vector<String> parameters)
{
    String message = parameters[2];
	unsigned int i = 3;

    if (message[0] == ':')
        message = message.substr(1);
    while (i < parameters.size())
    {
        message += " ";
        message += parameters[i];
        i++;
    }
    message = carriage_return(message);
    return message;
}

int Server::change_Message(std::vector<String> parameters, Client &client_side)
{
    if (parameters.size() < 3)
    {
        client_side.reply_to_message("need more parameters");
        return -1;
    }
    String  message = get_Message(parameters);
    try
    {
        std::vector<Channel>::iterator channel = find_a_Channel(parameters[1]);
        if (is_client_in_Channel(*channel, client_side.getFd()))
		{
            channel->broadcast_message(rpl_private_message(client_side, parameters[1], message), client_side);
		}
        else
		{
            client_side.reply_to_message("404 " + client_side.get_Nick_name() + " your not in the channel " + channel->get_Name());
		}
    }
    catch(const std::exception& e)
    {
       client_side.reply_to_message(e.what());
    }
    return 0;
}

int Server::private_message_Command(std::vector<String> parameters, Client &client_side)
{
    std::cout << "ENTER IN PRIVATE MESSAGE" << std::endl;

    if (client_side.get_State() != REGISTERED)
    {
        client_side.reply_to_message("you need to register first");
        return -1;
    }
    if (parameters.size() < 3)
	{
        client_side.reply_to_message("461 " + client_side.get_Nick_name() + " " + "Private Message" + " - Not enough parameters");
		return -1;
	}
    if (parameters[1].at(0) == '#')
	{
        return (change_Message(parameters, client_side));
	}
    try
    {
        Client  recipient = find_Client(parameters[1]);      
        String  message = get_Message(parameters);
        std::cout << "message = " << "[" << message << "]" << std::endl;
        String packet = rpl_private_message(cl, recipient.get_Nick_name(), message);
        std::cout << packet << recipient.getFd() << std::endl;
        packet += "\r\n";
        
		if (send(recipient.getFd(), packet.c_str(), packet.length(), 0) < 0)
		{
            throw std::out_of_range("error while sending in private message");
		}
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}