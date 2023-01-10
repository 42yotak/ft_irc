#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

Client::Client() {
	this->_nickName = "(nickname)";
	this->_userName = "(username)";
	this->buf_read[0] = '\0';
	this->buf_write[0] = '\0';
}

Client::~Client() {}

char *Client::getBufRead() {
	return this->buf_read;
}

char *Client::getBufWrite() {
	return this->buf_write;
}

void Client::setBufRead(char *buffer) {
	strlcat(this->buf_read, buffer, strlen(buffer));
}

void Client::setBufWrite(char *buffer) {
	strlcat(this->buf_write, buffer, strlen(buffer));
}

void Client::makeProtocol() {
	std::vector<std::string> cmds;

	cmds = split(std::string(this->buf_read), "\r\n");
	// Welcome
	// CAP LS \r\n
	// PASS \r\n
	// USER \r\n
	// NICK \r\n

	// Join
	// Join #channel \r\n
	std::vector<std::string>::iterator it = cmds.begin();
	std::vector<std::string>::iterator ite = cmds.end();
	while (it != ite) {
		std::vector<std::string> cmd = split((*it), " ");
		if (cmd[0] == "CAP") {
			callCommand()->cmdWelcome(cmds, this->buf_write);
		} else if (cmd[0] == "NICK") {
			callCommand()->cmdNick(cmds, this->buf_write);
		} else if (cmd[0] == "PING") {
			callCommand()->cmdPing(cmds, this->buf_write);
		} else if (cmd[0] == "QUIT") {
			callCommand()->cmdQuit(cmds, this->buf_write);
		} else if (cmd[0] == "JOIN") {
			callCommand()->cmdJoin(cmds, this->buf_write);
		} else if (cmd[0] == "PART") {
			callCommand()->cmdPart(cmds, this->buf_write);
		} else if (cmd[0] == "KICK") {
			callCommand()->cmdKick(cmds, this->buf_write);
		} else if (cmd[0] == "NOTICE") {
			callCommand()->cmdNotice(cmds, this->buf_write);
		} else if (cmd[0] == "PRIVMSG") {
			callCommand()->cmdPrivmsg(cmds, this->buf_write);
		} else {
			std::cout << RED "MSG ERROR" NC << std::endl;
		}
		++it;
	}
}