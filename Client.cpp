#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Utils.hpp"

Client::Client(int fd) {
	std::cout << BLUE "client is created" NC << std::endl;
	this->_fd = fd;

	this->_isRegistered = DEFAULT;
	this->buf_read[0] = '\0';
	this->buf_write[0] = '\0';

	this->_nickName = "*";
	this->_userName = "*";
	this->_realName = "*";

	this->_isDead = false;
}

Client::~Client() {
	//client 삭제할 때 할당된 fd반환
	this->_isDead = true;
	this->_channels.clear();
}

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

bool Client::getIsDead() const {
	return this->_isDead;
}

std::map<std::string, Channel *>&	Client::getChannels() {
	return this->_channels;
}

void Client::setBufWrite(const std::string &msg) {
	// strlcat(this->buf_write, msg, strlen(this->buf_read));
	this->buf_write += msg;
}

void Client::setIsRegistered(int cmd) {
	this->_isRegistered |= cmd;
	std::cout << '\t' << this->_isRegistered << std::endl;
<<<<<<< HEAD
}

void Client::offIsRegistered(int cmd) {
	this->_isRegistered &= ~cmd;
	std::cout << '\t' << this->_isRegistered << std::endl;
=======
>>>>>>> f6039ecc71a650c719ffe14e9c93796f4a9bd03e
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

void Client::setIsDead() {
	this->_isDead = true;
}

void Client::makeProtocol() {
	size_t delimiter;
	while ((delimiter = std::min(this->buf_read.find('\r'), this->buf_read.find('\n'))) != std::string::npos) {
		std::string cmd = this->buf_read.substr(0, delimiter);
		std::cout << "makeProtocol: [" << cmd << "]\n";
		this->buf_read.erase(0, delimiter + 1);
		if (cmd == "") continue;

		std::vector<std::string> tokens = split(cmd, " ");
		if (getIsRegistered() != (PASS | NICK | USER)) {
			callCommand()->welcomeProtocol(tokens, this);
		} else if (tokens[0] == "NICK") {
			callCommand()->cmdNick(tokens, this);
		} else if (tokens[0] == "PING") {
			callCommand()->cmdPing(tokens, this);
		} else if (tokens[0] == "QUIT") {
			callCommand()->cmdQuit(tokens, this);
		} else if (tokens[0] == "JOIN") {
			callCommand()->cmdJoin(tokens, this);
		} else if (tokens[0] == "PART") {
			callCommand()->cmdPart(tokens, this);
		} else if (tokens[0] == "KICK") {
			callCommand()->cmdKick(tokens, this);
		} else if (tokens[0] == "NOTICE") {
			callCommand()->cmdNotice(tokens, this);
		} else if (tokens[0] == "PRIVMSG") {
			callCommand()->cmdPrivmsg(tokens, this);
		} else {
			std::cout << RED "NOT CORRECT COMMAND" NC << std::endl;
		}
		// if not registered!
		
	}
}

void 	Client::addChannel(const std::string& name, Channel* channel) {
	this->_channels.insert(std::make_pair(name, channel));
}

void 	Client::removeChannel(const std::string& name) {
	this->_channels.erase(name);
}

void Client::clearBufRead() {
	this->buf_read.clear();
}
void Client::clearBufWrite() {
	this->buf_write.clear();
}