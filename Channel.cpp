#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() {}

Channel::Channel(std::string name) : _name(name) {}

Channel::~Channel() {
	_clients.clear();
}
