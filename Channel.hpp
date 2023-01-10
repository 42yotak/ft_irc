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
};
