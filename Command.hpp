#pragma once

#include <string>
#include <vector>
class Command {
	public:
		Command();
		~Command();

		void cmdWelcome(std::vector<std::string> buf_read, char *buf_write);
		void cmdPass(std::vector<std::string> buf_read, char *buf_write);
		void cmdNick(std::vector<std::string> buf_read, char *buf_write);
		void cmdUser(std::vector<std::string> buf_read, char *buf_write);
		void cmdPing(std::vector<std::string> buf_read, char *buf_write);
		void cmdQuit(std::vector<std::string> buf_read, char *buf_write);

		void cmdJoin(std::vector<std::string> buf_read, char *buf_write);
		void cmdPart(std::vector<std::string> buf_read, char *buf_write);
		void cmdKick(std::vector<std::string> buf_read, char *buf_write);

		void cmdNotice(std::vector<std::string> buf_read, char *buf_write);
		void cmdPrivmsg(std::vector<std::string> buf_read, char *buf_write);
};