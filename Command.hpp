#pragma once

#include <string>
#include <vector>

class Client;

class Command {
	public:
		Command();
		~Command();

		void welcomeProtocol(std::vector<std::string> cmds, Client *client);
		void cmdPass(std::vector<std::string> cmd, Client *client);
		void cmdNick(std::vector<std::string> cmd, Client *client);
		void cmdUser(std::vector<std::string> cmd, Client *client);
		void cmdPing(std::vector<std::string> cmd, Client *client);
		void cmdQuit(std::vector<std::string> cmd, Client *client);
		// void cmdWho(std::vector<std::string> cmd, Client *client);

		void cmdJoin(std::vector<std::string> cmd, Client *client);
		void cmdPart(std::vector<std::string> cmd, Client *client);
		void cmdKick(std::vector<std::string> cmd, Client *client);

		void cmdNotice(std::vector<std::string> cmd, Client *client);
		void cmdPrivmsg(std::vector<std::string> cmd, Client *client);

	private:
		bool isValidName(const std::string &name);
};
