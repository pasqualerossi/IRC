#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include "Client.hpp"
# include "Server.hpp"

typedef std::string String;

class Client;

class Channel 
{
	private:
		String						_name;
		String						_topic;
		int							_fdOp;
		size_t						_limit;
		String						_password;
		std::vector<Client>			_clients;

	public:
		Channel(std::string _name);
		~Channel();

	// Getter
		int						getFdOp() const; 
		size_t					getLimit() const;
		String					getName() const;
		String					getTopic() const;
		String					getPassword() const;
		std::vector<Client>		&getClients();
	// Setter
		void					setTopic(String newTopic);
		void					setPassword(String pass);
		void					setFdOp(int fd);
		void					setLimit(size_t limit);
	// Channel functions
		void					addClient(Client &client);
		void					eraseClient(Client &cl);
		void					broadcast(std::string message);
		void					broadcast(std::string message, Client &cl);
		void					debug();
};

#endif