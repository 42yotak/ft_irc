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
		client->offIsRegistered(PASS);
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
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" USER :Not enough parameters\r\n");
		return ;
		return ;
	}
	if (client->getIsRegistered() & USER) {
		// :irc.local 462 * :You may not reregister
		client->setBufWrite("462 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" :You may not reregister\r\n");
		return ;
	}
	if (!isValidName(cmd[1])) {
		// :irc.local 468 * USER :Your username is not valid'
		client->setBufWrite("468 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" USER :Your username is not valid\r\n");
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
	client->setBufWrite("Error :");
	client->setBufWrite("Closing link: ");
	client->setBufWrite("(" + client->getNickName() + ")");
	client->setBufWrite("[Quit: " + cmd[1] + "]\r\n");
	// broadcasting 127.000.000.001.06664-127.000.000.001.38276: :quit_ted!root@127.0.0.1 QUIT :Quit: leaving
	std::map<std::string, Channel *>::iterator channel = client->getChannels().begin();
	std::map<std::string, Channel *>::iterator ite = client->getChannels().end();
	std::string broadcastMsg(":" + client->getNickName() + " QUIT :Quit: " + cmd[1]);
	while (channel != ite) {
		(*channel).second->broadcast(client, broadcastMsg);
		(*channel).second->removeClient(client->getFd());
		++channel;
	}
	client->setIsDead();
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
		chan->broadcast(client, ":");
		chan->broadcast(client, client->getNickName());
		chan->broadcast(client, " JOIN :");
		chan->broadcast(client, *it);
		chan->broadcast(client, "\r\n");

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
	// 461
	if (cmd.size() < 2) {
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" PART :Not enough parameters\r\n");
		return ;
	}
	// 403 yotak <channelname> :No such channel
	std::vector<std::string> channels = split(cmd[1], ",");
	std::vector<std::string>::iterator it;
	
	for (it = channels.begin(); it != channels.end(); it++) {
		if (Server::callServer().isExistChannel(*it)) {
			if (client->getChannels().find(*it) == client->getChannels().end()) {
				// can't find
				// 442 jerry #42cluster :You're not on that channel
					client->setBufWrite("442 ");
					client->setBufWrite(client->getNickName());
					client->setBufWrite(" ");
					client->setBufWrite(*it);
					client->setBufWrite(" :You're not on that channel\r\n");
			} else {
				// set message
				// :yotak PART :#gun
				// :yotak PART #gun :bye
				Channel *chan = client->getChannels().find(*it)->second;
				chan->broadcast(client, ":");
				chan->broadcast(client, client->getNickName());
				chan->broadcast(client, " PART ");
				if (cmd.size() == 2) {
					chan->broadcast(client, " :");
					chan->broadcast(client, (*it));
				} else {
					chan->broadcast(client, (*it));
					chan->broadcast(client, " :");
					chan->broadcast(client, cmd[2]);
				}
				chan->broadcast(client, "\r\n");
				
				chan->removeClient(client->getFd());
				client->removeChannel(chan->getChannelName());
			}
		} else {
			client->setBufWrite("403 ");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" ");
			client->setBufWrite((*it));
			client->setBufWrite(" :No such channel\r\n");
		}
	}
}

void Command::cmdKick(std::vector<std::string> cmd, Client *client) {
	if (cmd.size() < 3) {
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" KICK :Not enough parameters\r\n");
		return ;
	}
	
	if (!Server::callServer().isExistChannel(cmd[1])) {
			client->setBufWrite("403 ");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" ");
			client->setBufWrite(cmd[1]);
			client->setBufWrite(" :No such channel\r\n");
		return;
	}

	Channel*									chan = Server::callServer().getChannel(cmd[1]);
	if (!(client->getNickName() == chan->getClients().begin()->second->getNickName())) {
		// 482(<client> <channel> :You're not channel operator) 
		// :irc.atw-inter.net 482 yuje #4242 :You're not channel operator
		client->setBufWrite("482 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" ");
		client->setBufWrite(cmd[1]);
		client->setBufWrite(" :You're not channel operator\r\n");
		return ;
	}

	std::vector<std::string>	users = split(cmd[2], ",");
	std::vector<std::string>::iterator it;
	for (it = users.begin(); it != users.end(); it++) {
		Client* banned = chan->searchNickname(*it);
		if (!banned) {
			// 441(<client> <nick> <channel> :They aren't on that channel)
			client->setBufWrite("441 ");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" ");
			client->setBufWrite(*it);
			client->setBufWrite(" ");
			client->setBufWrite(chan->getChannelName());
			client->setBufWrite(" :They aren't on that channel\r\n");
			continue ;
		}
		chan->broadcast(client, ":");
		chan->broadcast(client, client->getNickName());
		chan->broadcast(client, " KICK ");
		chan->broadcast(client, chan->getChannelName());
		if (cmd.size() != 4) {
			chan->broadcast(client, " :");
			chan->broadcast(client, *it);
		} else {
			chan->broadcast(client, " ");
			chan->broadcast(client, *it);
			chan->broadcast(client, " :");
			chan->broadcast(client, cmd[3]);
		}
		chan->broadcast(client, "\r\n");
		
		chan->removeClient(client->getFd());
		banned->removeChannel(chan->getChannelName());
	}
}

void Command::cmdPrivmsg(std::vector<std::string> cmd, Client *client) {
	/* PRIVMSG 401 404 461
		:yujerry PRIVMSG yotak :bye
		461 not enough parameter
		401 yotak #4242cluster2 :No such nick/channel
		404 user2 #42seoul :You cannot send external messages to this channel whilst the +n (noextmsg) mode is set.
	*/
	if (cmd.size() < 3) {
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" PRIVMSG :Not enough parameters\r\n");
		return ;
	}
	std::vector<std::string> targets = split(cmd[1], ",");

	std::vector<std::string>::iterator ite = targets.end();
	for (std::vector<std::string>::iterator target = targets.begin(); target != ite; target++) {
		if ((*target)[0] == '#') {
			// channel
			//401 yotak #4242cluster2 :No such nick/channel
			if (!Server::callServer().isExistChannel(*target)) {
				client->setBufWrite("401 ");
				client->setBufWrite(client->getNickName());
				client->setBufWrite(" ");
				client->setBufWrite(*target);
				client->setBufWrite(" :No such channel\r\n");
				continue;
			}
			std::map<std::string, Channel *>::iterator chanIt = client->getChannels().find(*target);
			// 404 user2 #42seoul :You askjhflasdkjgh
			if (chanIt == client->getChannels().end()) {
				client->setBufWrite("404 ");
				client->setBufWrite(client->getNickName());
				client->setBufWrite(" ");
				client->setBufWrite(*target);
				client->setBufWrite(" :You cannot send external messages to this channel\r\n");
				continue;
			}
			chanIt->second->broadcast(client, ":");
			chanIt->second->broadcast(client, client->getNickName());
			chanIt->second->broadcast(client, " PRIVMSG ");
			chanIt->second->broadcast(client, *target);
			chanIt->second->broadcast(client, " ");
			chanIt->second->broadcast(client, cmd[2]);
			chanIt->second->broadcast(client, "\r\n");
		} else {
			// user
			//401 yotak #4242cluster2 :No such nick/channel
			if (!Server::callServer().isUsedNickname(*target)) {
				client->setBufWrite("401 ");
				client->setBufWrite(client->getNickName());
				client->setBufWrite(" ");
				client->setBufWrite(*target);
				client->setBufWrite(" :No such nick\r\n");
				continue;
			}
			//:yujerry PRIVMSG yotak :bye
			client->setBufWrite(":");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" PRIVMSG ");
			client->setBufWrite(*target);
			client->setBufWrite(" :");
			client->setBufWrite(cmd[2]);
			client->setBufWrite("\r\n");
			Client* receiver = Server::callServer().getClient(*target);
			receiver->setBufWrite(":");
			receiver->setBufWrite(client->getNickName());
			receiver->setBufWrite(" PRIVMSG ");
			receiver->setBufWrite(*target);
			receiver->setBufWrite(" :");
			receiver->setBufWrite(cmd[2]);
			receiver->setBufWrite("\r\n");
		} 
	}
}

void Command::cmdNotice(std::vector<std::string> cmd, Client *client) {
	if (cmd.size() < 3) {
		client->setBufWrite("461 ");
		client->setBufWrite(client->getNickName());
		client->setBufWrite(" PRIVMSG :Not enough parameters\r\n");
		return ;
	}
	std::vector<std::string> targets = split(cmd[1], ",");

	std::vector<std::string>::iterator ite = targets.end();
	for (std::vector<std::string>::iterator target = targets.begin(); target != ite; target++) {
		if ((*target)[0] == '#') {
			// channel
			//401 yotak #4242cluster2 :No such nick/channel
			if (!Server::callServer().isExistChannel(*target)) {
				client->setBufWrite("401 ");
				client->setBufWrite(client->getNickName());
				client->setBufWrite(" ");
				client->setBufWrite(*target);
				client->setBufWrite(" :No such channel\r\n");
				continue;
			}
			std::map<std::string, Channel *>::iterator chanIt = client->getChannels().find(*target);
			// 404 user2 #42seoul :You askjhflasdkjgh
			if (chanIt == client->getChannels().end()) {
				client->setBufWrite("404 ");
				client->setBufWrite(client->getNickName());
				client->setBufWrite(" ");
				client->setBufWrite(*target);
				client->setBufWrite(" :You cannot send external messages to this channel\r\n");
				continue;
			}
			chanIt->second->broadcast(client, ":");
			chanIt->second->broadcast(client, client->getNickName());
			chanIt->second->broadcast(client, " NOTICE ");
			chanIt->second->broadcast(client, *target);
			chanIt->second->broadcast(client, " :");
			chanIt->second->broadcast(client, cmd[2]);
			chanIt->second->broadcast(client, "\r\n");
		} else {
			// user
			//401 yotak #4242cluster2 :No such nick/channel
			if (!Server::callServer().isUsedNickname(*target)) {
				client->setBufWrite("401 ");
				client->setBufWrite(client->getNickName());
				client->setBufWrite(" ");
				client->setBufWrite(*target);
				client->setBufWrite(" :No such nick\r\n");
				continue;
			}
			//:yujerry PRIVMSG yotak :bye
			client->setBufWrite(":");
			client->setBufWrite(client->getNickName());
			client->setBufWrite(" NOTICE ");
			client->setBufWrite(*target);
			client->setBufWrite(" :");
			client->setBufWrite(cmd[2]);
			client->setBufWrite("\r\n");
			Client* receiver = Server::callServer().getClient(*target);
			receiver->setBufWrite(":");
			receiver->setBufWrite(client->getNickName());
			receiver->setBufWrite(" NOTICE ");
			receiver->setBufWrite(*target);
			receiver->setBufWrite(" :");
			receiver->setBufWrite(cmd[2]);
			receiver->setBufWrite("\r\n");
		} 
	}
}
