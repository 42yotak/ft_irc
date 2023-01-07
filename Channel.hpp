#pragma once

#include <map>
#include <string>

class Client;

class Channel {
	private:
		std::string									_name;
		// 현재 채널에 참가한 유저 정보
		std::map<int, Client>				_clients;
		//방장 map.begin()
	public:
		Channel(std::string name);
		~Channel();
};