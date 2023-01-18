#pragma once

#include <string>
#include <map>
#include <utility>
#include <vector>

#include <sys/select.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <arpa/inet.h>
 #include <fcntl.h>

#include <iostream>
#include <exception>

#include <unistd.h>
#include <cstring>

#define ERROR -1

class Client;
class Channel;

class Server {
	private:
		static Server *_server;

		std::map<int, Client *>						_clients;
		std::map<std::string, Channel *>	_channels;
		fd_set														_readFds;
		fd_set														_writeFds;
		int																_servSock;
		std::string												_port;
		std::string												_password;

		Server();
		Server(std::string port, std::string password);

	public:
		static Server &callServer(std::string port = "6667", std::string password = "0000") {
			if (_server == NULL) {
				_server = new Server(port, password);
			}
			return *_server;
		}

		void on(std::string port, std::string password);
		void on();
		void off();

		int servRecv(int fd, std::string &buf_read);
		void servSend(int fd, std::string &buf_write);

		std::string	getPassword() const;

		Channel*	getChannel(const std::string& name);
		Client*		getClient(const std::string& nick);

		void removeClient(int fd);

		bool isUsedNickname(const std::string &nick);
		bool isExistChannel(const std::string &name);

		~Server();
};
