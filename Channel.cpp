#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() {}

Channel::Channel(std::string name) : _name(name) {}

Channel::~Channel() {
	_clients.clear();
}

std::string	&Channel::getChannelName() {
	return this->_name;
}

std::map<int, Client *>& Channel::getClients() {
	return this->_clients;
}

void Channel::broadcast(Client* self, const std::string &msg) {
	std::map<int, Client*>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		if ((*it).second != self)
			(*it).second->setBufWrite(msg);
	}
}

void Channel::addClient(int fd, Client* client) {
	this->_clients.insert(std::make_pair(fd, client));
}

void Channel::removeClient(int fd) {
	this->_clients.erase(fd);
}

Client* Channel::searchNickname(const std::string& nick) {
	std::map<int, Client *>::iterator ite = this->_clients.end();
	for (std::map<int, Client *>::iterator it = this->_clients.begin(); it != ite; it++) {
		if (it->second->getNickName() == nick) {
			return it->second;
		}
	}
	return NULL;
}