#include "Command.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Utils.hpp"
#include "Channel.hpp"

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
		//451 <client> :You have not registered
		client->setBufWrite("451 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite("  :You have not registered\r\n");
	}
	if (client->getIsRegistered() >= (NICK | USER)) {
		if (client->getIsRegistered() & PASS) {
			// buf_write에 welcome message 쓰기
			// <source> 001 :<client> :Welcome to the <networkname> Network, <nick>
			client->setBufWrite("001 ");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" :Welcome to the PokémonGo Network, ");
			client->setBufWrite(client->getNickName());
			client->setBufWrite("\r\n");
		} else {
			// ERROR :Closing link: [Access denied by configuration]
			client->setBufWrite("ERROR :Closing link: [Access denied by configuration]\r\n");
		}
	}
}

void Command::cmdPass(std::vector<std::string> cmd, Client *client) {
	if (cmd.size() < 2) {
		//"<client> <command> :Not enough parameters"
		//461 :nickname PASS :Not enough parameters
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" PASS :Not enough parameters\r\n");
		return ;
	}
	if (cmd[1] == Server::callServer().getPassword())
		client->setIsRegistered(PASS);
	else
		client->setIsRegistered(~PASS);
}

void Command::cmdNick(std::vector<std::string> cmd, Client *client) {
	if (cmd.size() < 2) {
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" NICK :Not enough parameters\r\n");
		return;
	}
	if (!this->isValidName(cmd[1])) {
		client->setBufWrite("432 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" ");
		client->setBufWrite(cmd[1]);
		client->setBufWrite(" :Erroneus nickname\r\n");
		return;
	}
	if (Server::callServer().isUsedNickname(cmd[1])) {
		//"433 :<client> <nick> :Nickname is already in use"
		client->setBufWrite("433 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" ");
		client->setBufWrite(cmd[1]);
		client->setBufWrite(" :Nickname is already in use\r\n");
		return;
	}
	client->setIsRegistered(NICK);
	if (client->getIsRegistered() == REGISTER) {
		client->setBufWrite(":");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" NICK ");
		client->setBufWrite(cmd[1]);
		client->setBufWrite("\r\n");
	}
	client->setNickName(cmd[1]);
}

bool Command::isValidName(const std::string& name) {
	// https://modern.ircdocs.horse/#clients
	if (name.find(' ') != std::string::npos ||
	name.find(',') != std::string::npos ||
	name.find('*') != std::string::npos ||
	name.find('?') != std::string::npos ||
	name.find('!') != std::string::npos ||
	name.find('@') != std::string::npos ||
	name.find('.') != std::string::npos ||
	name[0] == '$' || name[0] == ':' || name[0] == '#' || name[0] == '&')
		return false;
	return true;
}

void Command::cmdUser(std::vector<std::string> cmd, Client *client) {
	// yotak1
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
}

void Command::cmdPing(std::vector<std::string> cmd, Client *client) {
	if (cmd.size() < 2) {
		//not enough parameters
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" PING :Not enough parameters\r\n");
		return ;
	}
	client->setBufWrite(":");
	client->setBufWrite(SERVER_NAME);
	client->setBufWrite(" PONG :");
	client->setBufWrite(cmd[1]);
	client->setBufWrite("\r\n");
}

void Command::cmdQuit(std::vector<std::string> cmd, Client *client) {
	// QUIT [<message>]
	// QUIT 후 메시지가 없다면 QUIT :leaving
	// QUIT 뒤는 무조건 메시지 처리 
	/* 
	* 127.000.000.001.54630-127.000.000.001.06667: QUIT :#42seoul
	* 127.000.000.001.06667-127.000.000.001.54630: ERROR :Closing link: (root@127.0.0.1) [Quit: #42seoul]
	*/
	if (cmd.size() == 1) {
		cmd.push_back(std::string("Client exited"));
	}
	client->setIsDead();
	client->setBufWrite("Error :");
	client->setBufWrite("Closing link: ");
	client->setBufWrite("(" + client->getNickName() + ")");
	client->setBufWrite("[Quit: " + cmd[1] + "]\r\n");
	// broadcasting 127.000.000.001.06664-127.000.000.001.38276: :quit_ted!root@127.0.0.1 QUIT :Quit: leaving
	std::map<std::string, Channel *>::iterator it = client->getChannels().begin();
	std::map<std::string, Channel *>::iterator ite = client->getChannels().end();
	std::string broadcastMsg(":" + client->getNickName() + " QUIT :Quit: " + cmd[1]);
	while (it != ite) {
		(*it).second->broadcast(broadcastMsg);
		++it;
	}
}

void Command::cmdJoin(std::vector<std::string> cmd, Client *client) {
	/* 
	NUMERIC JOIN 461 403 353 366
	EXAMPLE :<UserJID> JOIN :<ChannelName>
	*/
	std::vector<std::string> channels = split(cmd[1], ",");
	std::vector<std::string>::iterator it;
	
	for (it = channels.begin(); it != channels.end(); it++) {
		Channel* chan = Server::callServer().getChannel(*it);
		// channel에 client 추가
		chan->addClient(client->getFd(), client);
		client->addChannel(*it, chan);

		// channel에 있는 모든 client한테 joined msg 출력
		chan->broadcast(":");
		chan->broadcast(client->getNickName());
		chan->broadcast(" JOIN :");
		chan->broadcast(*it);
		chan->broadcast("\r\n");

		// client한테 출력
			// 353 :<client> <symbol> <channel> :[prefix]<nick>{ [prefix]<nick>}
		client->setBufWrite("353 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" = ");
		client->setBufWrite(*it);
		client->setBufWrite(" :@");
		std::map<int, Client *>::iterator tmp = chan->getClients().begin();
		client->setBufWrite(tmp->second->getNickName());
		tmp++;
		for(; tmp != chan->getClients().end(); tmp++) {
			client->setBufWrite(" ");
			client->setBufWrite(tmp->second->getNickName());
		}
		client->setBufWrite("\r\n");

		// 366 :<client> <channel> :End of /NAMES list
		client->setBufWrite("366 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" ");
		client->setBufWrite(*it);
		client->setBufWrite(" :End of /NAMES list\r\n");	
	}
}

void Command::cmdPart(std::vector<std::string> cmd, Client *client) {
	/*
	NUMERIC 461 403 442
	*/
	//:irc.local 403 yotak <channelname> :No such channel

	// 내가 없는 채널에서 나가려고 하는경우 442
	// 127.000.000.001.60774-127.000.000.001.06664: PART #42cluster
	// 127.000.000.001.06664-127.000.000.001.60774: :irc.local 442 jerry #42cluster :You're not on that channel

	// 127.000.000.001.44682-127.000.000.001.06664: PART #gun,#gon
	// 127.000.000.001.06664-127.000.000.001.44682: :yotak!root@127.0.0.1 PART :#gun
	// :yotak!root@127.0.0.1 PART :#gon

	// 127.000.000.001.44682-127.000.000.001.06664: PART #gun,#gon

	// /part #gam, #lee
	// 127.000.000.001.44682-127.000.000.001.06664: PART #gam, :#lee

	// 127.000.000.001.06664-127.000.000.001.44682: :yotak!root@127.0.0.1 PART #gam :#lee

	// 127.000.000.001.06664-127.000.000.001.60770: :yotak!root@127.0.0.1 PART #hellocluster :hellocluster bye

	//broadcast 메시지와 나에게 보내는 메시지가 같다!
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