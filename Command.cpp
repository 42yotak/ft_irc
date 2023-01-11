#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"

Command::Command() {

}

Command::~Command() {

}

void Command::welcomeProtocol(std::vector<std::string> cmds, Client *client) {
	if (cmds[0] == "PASS") {
		this->cmdPass(cmds, client);
	} else if (cmds[0] == "USER") {
		this->cmdPass(cmds, client);
	} else if (cmds[0] == "NICK") {
		this->cmdPass(cmds, client);
	} else if (cmds[0] == "CAP") {
		//ignore
	} else {
		//:irc.local 451 * KICK :You have not registered.
	}

	if (client->getIsRegistered() & (NICK | USER) == (NICK | USER)) {
		if (client->getIsRegistered() & PASS) {
			// buf_write에 welcome message 쓰기
		} else {
			// ERROR Closing link: (root@127.0.0.1) [Access denied by configuration]
			Server::callServer().removeClient(client->getFd());
		}
	}
}

void Command::cmdPass(std::vector<std::string> cmd, Client *client) {
	if (cmd[1] == Server::callServer().getPassword())
		client->setIsRegistered(PASS);
}

void Command::cmdNick(std::vector<std::string> cmd, Client *client) {
// yuhwang
}

bool isValidName(const std::string& name) {
	// https://modern.ircdocs.horse/#clients
	if (name.find(' ') || name.find(',') || name.find('*') || \
			name.find('?') || name.find('!') || name.find('@') || \
			name[0] == '$' || name[0] == ':' || name[0] == '#' || \
			name[0] == '&' || name.find('.'))
		return false;
	return true;
}

void Command::cmdUser(std::vector<std::string> cmd, Client *client) {
	// USER root 0 * :root
	// The minimum length of <username> is 1, ie. it MUST NOT be empty.
	// isVerified 검사 완료시 client의 buffer에 write
	// USER [username] [nickname] [address] [:realname]
	if (cmd.size() < 5) {
		// Severname errono nickname errormsg
		//:irc.local 461 * USER :Not enough parameters
		return ;
	}
	if (client->getIsRegistered() & USER) {
		// irc.your_domain_name
		// :irc.local 462 * :You may not reregister
		return ;
	}
	if (!isValidName(cmd[1])) {
		// :irc.local 468 * USER :Your username is not valid
		return ;
	}
	client->setUserName(cmd[1]);
	client->setRealName(cmd[4]);
	client->setIsRegistered(USER);
}
void Command::cmdPing(std::vector<std::string> cmd, Client *client) {

}
void Command::cmdQuit(std::vector<std::string> cmd, Client *client) {

}

void Command::cmdJoin(std::vector<std::string> cmd, Client *client) {
	// 공백으로 split
	// comma로 split
	// comma로 split한 각각을 channel로
}
void Command::cmdPart(std::vector<std::string> cmd, Client *client) {

}
void Command::cmdKick(std::vector<std::string> cmd, Client *client) {

}

void Command::cmdNotice(std::vector<std::string> cmd, Client *client) {

}
void Command::cmdPrivmsg(std::vector<std::string> cmd, Client *client) {

}