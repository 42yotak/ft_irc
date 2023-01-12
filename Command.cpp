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
		this->cmdUser(cmds, client);
	} else if (cmds[0] == "NICK") {
		this->cmdNick(cmds, client);
	} else if (cmds[0] == "CAP") {
		//ignore
	} else {
		//:irc.local 451 * KICK :You have not registered.
	}

	if ((client->getIsRegistered() & (NICK | USER)) == (NICK | USER)) {
		if (client->getIsRegistered() & PASS) {
			// buf_write에 welcome message 쓰기
			// 001 :<client> :Welcome to the <networkname> Network, <nick>
			client->setBufWrite("001 :");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" :Welcome to the PokémonGo Network, ");
			client->setBufWrite(client->getNickName());
			client->setBufWrite("\r\n");
			// 002 :<client> :Your host is <servername>, running version <version>
			client->setBufWrite("002 :");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" :Your host is PokémonGo, running version 1.0\r\n");
			// 003 :<client> :This server was created <datetime>
			client->setBufWrite("003 :");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" :This server was created yesterday\r\n");
			// 004 :<client> <servername> <version> <available user modes> <available channel modes>
			client->setBufWrite("004 :");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" PokémonGo 1.0\r\n");
			// 005 :<client> <1-13 tokens> :are supported by this server
			client->setBufWrite("005 :");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" :are supported by this server\r\n");
		} else {
			// "<client> :Password incorrect"
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" ");
			client->setBufWrite(" :Password incorrect\r\n");
			Server::callServer().removeClient(client->getFd());
		}
	}
}

void Command::cmdPass(std::vector<std::string> cmd, Client *client) {
	if (cmd.size() < 2) {
		//"<client> <command> :Not enough parameters"
		//461 nickname PASS :Not enough parameters
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" PASS");
		client->setBufWrite(" :Not enough parameters\r\n");
	}
	if (cmd[1] == Server::callServer().getPassword())
		client->setIsRegistered(PASS);
	else
		client->setIsRegistered(~PASS);
}

void Command::cmdNick(std::vector<std::string> cmd, Client *client) {
// yuhwang
	if (cmd.size() < 2) {
		client->setBufWrite("461 :Not enough parameters\r\n");
		return;
	}
	if (!this->isValidName(cmd[1])) {
		client->setBufWrite("432 :");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" ");
		client->setBufWrite(cmd[1]);
		client->setBufWrite(" :Erroneus nickname\r\n");
		return ;
	}
	client->setNickName(cmd[1]);
	if (client->getIsRegistered() == REGISTER) {
		
	}
}

bool Command::isValidName(const std::string& name) {
	// https://modern.ircdocs.horse/#clients
	if (name.find(' ') || name.find(',') || name.find('*') || \
			name.find('?') || name.find('!') || name.find('@') || \
			name[0] == '$' || name[0] == ':' || name[0] == '#' || \
			name[0] == '&' || name.find('.'))
		return false;
	return true;
}

void Command::cmdUser(std::vector<std::string> cmd, Client *client) {
	// USER root * 0 :root
	// USER [username] [nickname] [address] [:realname]
	if (cmd.size() < 5) {
		// <command> :<reason>
		// :irc.local 461 * USER :Not enough parameters
		client->setBufWrite("461 :Not enough parameters\r\n");
		return ;
	}
	if (client->getIsRegistered() & USER) {
		// :irc.local 462 * :You may not reregister
		client->setBufWrite("462 :You may not reregister\r\n");
		return ;
	}
	if (!isValidName(cmd[1])) {
		// :irc.local 468 * USER :Your username is not valid'
		client->setBufWrite("468 :Your username is not valid\r\n");
		return ;
	}
	client->setUserName(cmd[1]);
	client->setRealName(cmd[4]);
	client->setIsRegistered(USER);
	if (client->getIsRegistered() == NOPASS) {
		client->setBufWrite("ERROR :Closing Link: " + client->getNickName() + \
			" [Access denied by configuration]\r\n");
		// client 제거
	}
}
void Command::cmdPing(std::vector<std::string> cmd, Client *client) {

}
void Command::cmdQuit(std::vector<std::string> cmd, Client *client) {
	// QUIT [<message>]
}

void Command::cmdJoin(std::vector<std::string> cmd, Client *client) {
	// 공백으로 split
	// comma로 split
	// comma로 split한 각각을 channel로
}
void Command::cmdPart(std::vector<std::string> cmd, Client *client) {

}
void Command::cmdKick(std::vector<std::string> cmd, Client *client) {
	// KICK <channel> <nicks> [<reason>]
	// ' ' split
	// if (cmd < 2) -> error
	// cmd[0] -> "channel"
	// cmd[1] -> ',' split
	// while(cmd[1][i]) -> "nickname"
	// if(cmd[2])
	// 	while(cmd[i]) -> "reason comment"
}

void Command::cmdNotice(std::vector<std::string> cmd, Client *client) {

}
void Command::cmdPrivmsg(std::vector<std::string> cmd, Client *client) {

}