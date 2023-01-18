/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_notices.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:27:21 by prossi            #+#    #+#             */
/*   Updated: 2023/01/18 18:39:46 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// NEAT UP!

String RPL_NOTICE(Client cl, String recipient, String message)
{

    return (":" + cl.get_Nick_name() + " NOTICE " + recipient + " :" + message);
}

String      getMessageNotice(std::vector<String> params)
{
    String message;

    unsigned int i = 3;
    message = params[2];
    if (message[0] == ':')
        message = message.substr(1);
    while (i < params.size())
    {
        message += " ";
        message += params[i];
        i++;
    }
    message = carriage_return(message); //enleve le \r
    return message;

}

int Server::change_Notice(std::vector<String> params, Client &cl)
{
    if (params.size() < 3)
    {
        cl.reply("need more params");
        return -1;
    }
    String  message = getMessageNotice(params);
    try
    {
        std::vector<Channel>::iterator chan = find_a_Channel(params[1]);
        if (isClientInChannel(*chan, cl.get_file_descriptor()))
            chan->broadcast_message(RPL_NOTICE(cl, params[1], message), cl);
        else
            cl.reply("404 " + cl.get_Nick_name() + " you are not in the channel " + chan->get_Name());
    }
    catch(const std::exception& e)
    {
       cl.reply(e.what());
    }
    return 0;
}

int Server::notice_Command(std::vector<String> params, Client &cl)
{
    std::cout << "ENTER IN NOTICE" << std::endl;
    if (cl.get_State() != REGISTERED)
    {
        cl.reply("you need to register first (prvmsg)");
        return -1;
    }
    if (params.size() < 3)
	{
        std::cout << "paramas size = "<< params.size() << std::endl;
        cl.reply("461 " + cl.get_Nick_name() + " " + "NOTICE" + " :Not enough parameters");
		return -1;
	}
    if (params[1].at(0) == '#')
        return (change_Notice(params, cl));
    try
    {
        Client  recipient = findClient(params[1]);      
        String  msg = getMessageNotice(params);
       // std::cout << "message = " << "[" << msg << "]" << std::endl;
        String paquet = RPL_NOTICE(cl, recipient.get_Nick_name(), msg);
        std::cout << paquet << recipient.get_file_descriptor() << std::endl;
        paquet += "\r\n";
        if (send(recipient.get_file_descriptor(), paquet.c_str(), paquet.length(), 0) < 0)
            throw std::out_of_range("error while sendig in notice");
    }
    catch(const std::exception& e)
    {
        cl.reply(e.what());
        std::cerr << e.what() << '\n';
    }

    return 0;
}