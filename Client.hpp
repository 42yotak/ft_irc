#pragma once

#include <map>
#include <iostream>
#include <string>
#include <exception>

#include <unistd.h>

#define PASS 1
#define NICK 2
#define USER 4
#define NOPASS 6
#define REGISTER 7

class Channel;

class Client {
	private:
		int																_fd;
		std::string												buf_read;
		std::string												buf_write;

		int																_isRegistered;
		std::string												_nickName;
		std::string												_userName;
		std::string												_realName;
		std::map<std::string, Channel *>	_channels;

	public:
		Client(int fd);
		~Client();

		std::string 			&getBufRead();
		std::string 			&getBufWrite();
		int								getIsRegistered() const;
		int								getFd() const;
		const std::string	&getNickName() const;
		const std::string	&getUserName() const;
		const std::string	&getRealName() const;

		// void	setBufRead(const std::string &msg);
		void	setBufWrite(const std::string &msg);
		void	setIsRegistered(int cmd);
		void	setNickName(const std::string &nickName);
		void	setUserName(const std::string &userName);
		void	setRealName(const std::string &realName);

		void	makeProtocol();

		// useless?
		void clearBufRead();
		void clearBufWrite();
};
