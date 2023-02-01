/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prossi <prossi@student.42adel.org.au>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2023/02/01 12:46:39 by prossi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

String RPL_NOTICE(Client cl, String recipient, String message)
{
    return (":" + cl.getNickname() + " NOTICE " + recipient + " :" + message);
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
    message = erasebr(message);
    return message;

}

int Server::chanNotice(std::vector<String> params, Client &cl)
{
    if (params.size() < 3)
    {
        cl.reply("need more params");
        return -1;
    }
    String  message = getMessageNotice(params);
    try
    {
        std::vector<Channel>::iterator chan = findChannelIt(params[1]);
        if (isClientInChannel(*chan, cl.getFd()))
            chan->broadcast(RPL_NOTICE(cl, params[1], message), cl);
        else
            cl.reply("404 " + cl.getNickname() + " you are not in the channel " + chan->getName());
    }
    catch(const std::exception& e)
    {
       cl.reply(e.what());
    }
    return 0;
}

int Server::cmdNotice(std::vector<String> params, Client &cl)
{
    std::cout << "ENTER IN NOTICE" << std::endl;
    if (cl.getState() != REGISTERED)
    {
        cl.reply("you need to register first (prvmsg)");
        return -1;
    }
    if (params.size() < 3)
	{
        std::cout << "paramas size = "<< params.size() << std::endl;
        cl.reply("461 " + cl.getNickname() + " " + "NOTICE" + " :Not enough parameters");
		return -1;
	}
    if (params[1].at(0) == '#')
        return (chanNotice(params, cl));
    try
    {
        Client  recipient = findClient(params[1]);      
        String  msg = getMessageNotice(params);
        String paquet = RPL_NOTICE(cl, recipient.getNickname(), msg);
        std::cout << paquet << recipient.getFd() << std::endl;
        paquet += "\r\n";
        if (send(recipient.getFd(), paquet.c_str(), paquet.length(), 0) < 0)
            throw std::out_of_range("error while sendig in notice");
    }
    catch(const std::exception& e)
    {
        cl.reply(e.what());
        std::cerr << e.what() << '\n';
    }
    return 0;
}