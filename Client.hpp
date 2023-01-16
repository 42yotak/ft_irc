#pragma once

#include <map>
#include <iostream>
#include <string>
#include <exception>

#include <unistd.h>

#define DEFAULT 0
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

		bool															_isDead;

	public:
		Client(int fd);
		~Client();

		std::string 											&getBufRead();
		std::string 											&getBufWrite();
		std::map<std::string, Channel *>	&getChannels();
		int																getIsRegistered() const;
		int																getFd() const;
		bool															getIsDead() const;
		const std::string									&getNickName() const;
		const std::string									&getUserName() const;
		const std::string									&getRealName() const;

		void	setBufWrite(const std::string &msg);
		void	setIsRegistered(int cmd);
		void	setNickName(const std::string &nickName);
		void	setUserName(const std::string &userName);
		void	setRealName(const std::string &realName);
		void	setIsDead();

		void	makeProtocol();

		void 	addChannel(const std::string& name, Channel* channel);
		void 	removeChannel(const std::string& name);

		// useless?
		void clearBufRead();
		void clearBufWrite();
};
