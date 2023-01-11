#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Utils.hpp"

Client::Client(int fd) {
	this->_fd = fd;
	this->_isRegistered = 0;
	this->buf_read[0] = '\0';
	this->buf_write[0] = '\0';
	this->_nickName = "*";
	this->_userName = "*";
	this->_realName = "*";
}

Client::~Client() {}

std::string &Client::getBufRead() {
	return this->buf_read;
}

std::string &Client::getBufWrite() {
	return this->buf_write;
}

int Client::getIsRegistered() const {
	return this->_isRegistered;
}

int Client::getFd() const {
	return this->_fd;
}

const std::string	&Client::getNickName() const {
	return this->_nickName;
}
const std::string	&Client::getUserName() const {
	return this->_userName;
}
const std::string	&Client::getRealName() const {
	return this->_realName;
}

void Client::setBufRead(const std::string &msg) {
	// strlcat(this->buf_read, msg, strlen(this->buf_read));
	this->buf_read += msg;
}

void Client::setBufWrite(const std::string &msg) {
	// strlcat(this->buf_write, msg, strlen(this->buf_read));
	this->buf_write += msg;
}

void Client::setIsRegistered(int cmd) {
	this->_isRegistered |= cmd;
}

void	Client::setNickName(const std::string &nickName) {
	this->_nickName = nickName;
}

void	Client::setUserName(const std::string &userName) {
	this->_userName = userName;
}

void	Client::setRealName(const std::string &realName) {
	this->_realName = realName;
}

void Client::makeProtocol() {
	// std::vector<std::string> cmds;

	// cmds = split(std::string(this->buf_read), "\r\n");
	std::vector<std::string> cmds(split(std::string(this->buf_read), "\r\n"));
	// cmds
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
		if (getIsRegistered() != (PASS | NICK | USER)) {
			callCommand()->welcomeProtocol(cmds, this);
		} else if (cmd[0] == "NICK") {
			callCommand()->cmdNick(cmds, this);
		} else if (cmd[0] == "PING") {
			callCommand()->cmdPing(cmds, this);
		} else if (cmd[0] == "QUIT") {
			callCommand()->cmdQuit(cmds, this);
		} else if (cmd[0] == "JOIN") {
			callCommand()->cmdJoin(cmds, this);
		} else if (cmd[0] == "PART") {
			callCommand()->cmdPart(cmds, this);
		} else if (cmd[0] == "KICK") {
			callCommand()->cmdKick(cmds, this);
		} else if (cmd[0] == "NOTICE") {
			callCommand()->cmdNotice(cmds, this);
		} else if (cmd[0] == "PRIVMSG") {
			callCommand()->cmdPrivmsg(cmds, this);
		} else {
			std::cout << RED "MSG ERROR" NC << std::endl;
		}
		// if not registered!
		
		++it;
	}
}

void Client::clearBufRead() {
	this->buf_read.clear();
}
void Client::clearBufWrite() {
	this->buf_write.clear();
}