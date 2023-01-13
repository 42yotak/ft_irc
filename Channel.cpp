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

void Channel::broadcast(const std::string &msg) {
	std::map<int, Client*>::iterator it;
	for (it = this->_clients.begin(); it != this->_clients.end(); it++) {
		(*it).second->setBufWrite(msg);
	}
}

void Channel::addClient(int fd, Client* client) {
	this->_clients.insert(std::make_pair(fd, client));
}