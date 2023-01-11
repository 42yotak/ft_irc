#pragma once

#include <map>
#include <iostream>
#include <string>
#include <exception>

class Channel;

class Client {
	private:
		char buf_read[512];
		char buf_write[512];
		std::string												_nickName;
		std::string												_userName;
		std::map <std::string, Channel *>	_channels;

	public:
		Client();
		~Client();

		char *getBufRead();
		char *getBufWrite();

		void setBufRead(char *buffer);
		void setBufWrite(char *buffer);

		void makeProtocol();
};
