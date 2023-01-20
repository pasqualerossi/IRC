/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_notices.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:21 by prossi            #+#    #+#             */
/*   Updated: 2023/01/20 10:39:24 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String rpl_Notice(Client client, String recipient, String message)
{

    return (":" + client.get_Nick_name() + " NOTICE " + recipient + " :" + message);
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

int Server::change_Notice(std::vector<String> parameters, Client &client)
{
    String  message = get_message_Notice(params);

	if (parameters.size() < 3)
    {
        client.reply("need more params");
        return -1;
    }
    try
    {
        std::vector<Channel>::iterator change = find_a_Channel(parameters[1]);
        if (isClientInChannel(*change, client.get_file_descriptor()))
		{
            change->broadcast_message(rpl_Notice(cl, parameters[1], message), client);
		}
		else
		{
            client.reply("404 " + client.get_Nick_name() + " you are not in the channel " + change->get_Name());
		}
    }
    catch(const std::exception& e)
    {
       client.reply(e.what());
    }
    return 0;
}

int Server::notice_Command(std::vector<String> parameters, Client &client)
{
    std::cout << "ENTER IN NOTICE" << std::endl;
    if (client.get_State() != REGISTERED)
    {
        client.reply("you need to register first (prvmsg)");
        return -1;
    }
    if (parameters.size() < 3)
	{
        std::cout << "paramas size = "<< parameters.size() << std::endl;
        client.reply("461 " + client.get_Nick_name() + " " + "NOTICE" + " :Not enough parameters");
		return -1;
	}
    if (parameters[1].at(0) == '#')
        return (change_Notice(parameters, client));
    try
    {
        Client  recipient = find_Client(parameters[1]);      
        String  message = get_message_Notice(parameters);
        String packet = rpl_Notice(client, recipient.get_Nick_name(), message);

        std::cout << packet << recipient.get_file_descriptor() << std::endl;
        packet += "\r\n";
        if (send(recipient.get_file_descriptor(), packet.c_str(), packet.length(), 0) < 0)
		{
            throw std::out_of_range("error while sendig in notice");
		}
    }
    catch(const std::exception& e)
    {
        client.reply(e.what());
        std::cerr << e.what() << '\n';
    }
    return 0;
}