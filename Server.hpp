#pragma once

#include <map>
#include <vector>
#include <sys/select.h> // fd_set
#include <sys/socket.h> // sockaddr_in
#include <netinet/in.h> // 이것도
#include <netinet/ip.h>  // 이것도

class Client;
class Channel;

class Server {
	private:
		std::map<int, Client*>	_clients;
		std::vector<Channel*>		_channels;
		fd_set									_readfds;
		fd_set									_writefds;
		int											_sockfd;
		struct sockaddr_in			_servAddr;

	public:
		void initSocket();
};