#include "../../headers/Server.hpp"

String	ERR_PASSWDMISMATCH(Client &client) {
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
			std::cout << "charatere numero : " << i << " RETOUR A LA LIGNE\n";
			return (1);
		}
		i++;
	}
	return 0;
}

int Server::cmdPass(std::vector<String> pass, Client &cl) {
	if (pass.size() < 2)
	{
		cl.reply(ERR_NEEDMOREPARAMS(cl, "PASS"));
		return -1;
	}
	String mdp = erasebr(pass[1]); // enleve le \r a la fin de pass
	if (mdp != _password)
	{
		std::cout <<"pass[1] : " << "[" << mdp << "]"<< std::endl;
		std::cout << "password : " << "[" << _password << "]" << std::endl;
		teststring(pass[1]);
		cl.reply(ERR_PASSWDMISMATCH(cl));
		return -1;
	}
	cl.setState(LOGIN);
	cl.welcome();
	return 0;
}