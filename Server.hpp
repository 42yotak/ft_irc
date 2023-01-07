#pragma once

#include <string>
#include <map>
#include <vector>

#include <sys/select.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>

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
		Server(std::string port, std::string password);
		~Server();
		void on(std::string port, std::string password); // 직접 세팅
		void on(void); //생성자로 들어오는 내부 변수 사용
		void off(); // close(sockfd)
		void freeVector(); // To.hyko 컨테이너 미리 익숙해지세요~
		void freeMap();
};