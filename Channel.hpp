#pragma once

#include <map>
#include <string>

class Client;

class Channel {
	private:
		std::string										_name;
		std::map<int, Client *>				_clients;

	public:
		Channel();
		Channel(std::string name);
		~Channel();

		std::string& getChannelName();
		std::map<int, Client *>& getClients();

		void broadcast(const std::string& msg);
		void addClient(int fd, Client* client);
		void removeClient(const std::string& nick);
};
