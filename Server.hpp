#pragma once

#include <string>
#include <map>
#include <vector>

#include <sys/select.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
#include <exception>

#include <unistd.h>
#include <cstring>

#define ERROR -1

class Client;
class Channel;

class Server {
	private:
		std::map<int, Client>							_clients;
		std::map<std::string, Channel>		_channels;
		fd_set														_readFds;
		fd_set														_writeFds;
		int																_servSock;
		struct sockaddr_in								_servAdr;
		std::string												_port;
		std::string												_password;

	public:
		Server(std::string port, std::string password);
		~Server();
		void on(std::string port, std::string password);
		void on();
		void off();
};