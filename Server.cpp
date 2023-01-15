#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

Server *Server::_server = NULL;

Server::Server(std::string port, std::string password) : _port(port), _password(password) {
	this->_servSock = 0;

	FD_ZERO(&this->_readFds);
	FD_ZERO(&this->_writeFds);
}

Server::Server() {
	this->_servSock = 0;

	FD_ZERO(&this->_readFds);
	FD_ZERO(&this->_writeFds);
}

Server::~Server() {
	this->_clients.clear();
	this->_channels.clear();
}

void Server::on(std::string port, std::string password) {
	(void) password;
	struct timeval timeout;
	struct sockaddr_in servAdr;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	if ((this->_servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == ERROR)
		throw std::runtime_error("socket ");
	int option = true;
	std::cout << "sockopt " << setsockopt( _servSock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option) ) << std::endl;
	std::cout << "this.servsock fron socket(): " << this->_servSock << std::endl;
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	std::cout << "port : " << atoi(port.c_str()) << std::endl;
	servAdr.sin_port = htons(atoi(port.c_str()));

	if (bind(this->_servSock, (struct sockaddr*)&servAdr, sizeof(servAdr)) == ERROR)
		throw std::runtime_error("bind ");
	if (listen(_servSock, 42) == ERROR)
		throw std::runtime_error("listen ");
	FD_SET(_servSock, &_readFds);

	fd_set	cpReadFds;
	fd_set	cpWriteFds;
	int	selectFds = 0;
	int	maxFd = this->_servSock;

	// for accept()
	struct sockaddr_in	clientAdr;
	int									clientLen, clientFd = 0;

	while(42) {
		FD_COPY(&this->_readFds, &cpReadFds);
		FD_COPY(&this->_writeFds, &cpWriteFds);

		try {
			if ((selectFds = select(maxFd + 1, &cpReadFds, NULL, NULL, &timeout)) == ERROR)
				throw std::runtime_error("select ");
		} catch(std::exception &e) {
			std::cerr << "errno: " << errno << std::endl;
			std::cerr << e.what() << std::endl;
		}
		std::cout << "selectFds: " << selectFds << std::endl;

		if (selectFds == 0)
			continue ;

		for (int fd = 0; fd < maxFd + 1; fd++) {

			// revc from client
			if (FD_ISSET(fd, &cpReadFds)) {
				// accept call
				if (fd == this->_servSock) {
					std::cout << "fd: " << fd << " servsock: " << this->_servSock << std::endl;

					try {
						clientFd = accept(fd, (struct sockaddr *)&clientAdr, (socklen_t *)&clientLen);
						if (clientFd < 0) {
							throw std::runtime_error("accept ");
						}
						std::cout << "clientfd: " << clientFd << std::endl;
						FD_SET(clientFd, &this->_readFds);
						FD_SET(clientFd, &this->_writeFds);

						Client *newClient = new Client(clientFd);
						this->_clients.insert(std::make_pair(clientFd, newClient));
						if (clientFd > maxFd) {
							maxFd = clientFd;
						}
					} catch(std::exception &e) {
						std::cerr << e.what() << std::endl;
					}
				} else {
					// TODO: read message
					Client *client = this->_clients[fd];
					this->servRecv(fd, client->getBufRead());
					if (_clients.find(fd) != _clients.end()) {
						client->makeProtocol();
						client->clearBufRead();
					}
				}
			}
			if (FD_ISSET(fd, &cpWriteFds)) {
				// std::cout << "write!!" << std::endl;
				std::map<int, Client*>::iterator clnt = this->_clients.find(fd);
				if (clnt != this->_clients.end()) {
					this->servSend(fd, clnt->second->getBufWrite());
					clnt->second->clearBufWrite();
					if (clnt->second->getIsDead())
						removeClient(fd);
				}
			}
		}
	}
}

void Server::on() {
	this->on(this->_port, this->_password);
}

void Server::off() {
	close(_servSock);
}

void Server::servRecv(int fd, std::string &buf_read) {
	char		buf[512];
	ssize_t	nbytes;

	buf[0] = '\0';
	try {
		nbytes = recv(fd, (void *)buf, 512, MSG_DONTWAIT);
		std::cout << "nbytes: " << nbytes << std::endl;
		if (nbytes > 510 || nbytes == ERROR) {
			throw(std::runtime_error("recv message "));
		}
		// close request
		if (nbytes == 0) {
			FD_CLR(fd, &this->_readFds);
			FD_CLR(fd, &this->_writeFds);
			this->removeClient(fd);
			return;
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	buf[nbytes] = '\0';

	buf_read += std::string(buf);
	std::cout << PURPLE << std::string(buf) << NC;
}

void Server::servSend(int fd, std::string &buf_write) {
	try {
		if (send(fd, buf_write.c_str(), buf_write.length(), 0)) {
			throw std::runtime_error("send message");
		}
		buf_write.clear();
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
}

std::string	Server::getPassword() const {
	return this->_password;
}

Channel* Server::getChannel(std::string &name) {
	std::map<std::string, Channel*>::iterator it = this->_channels.find(name);
	if (it != this->_channels.end()) {
		return (*it).second;
	}
	return this->_channels.insert(std::make_pair(name, new Channel(name))).first->second;
}

void Server::removeClient(int fd) {
	std::map<int, Client *>::iterator it = this->_clients.find(fd);
	if (it != this->_clients.end()) {
		delete (*it).second;
		this->_clients.erase(it);
		close(fd);
	}
	std::cout << RED "remove client end" NC << std::endl;
}

bool Server::isUsedNickname(const std::string &nick) {
	std::map<int, Client *>::iterator ite = this->_clients.end();
	for (std::map<int, Client *>::iterator it = this->_clients.begin(); it != ite; ++it) {
		if (it->second->getNickName() == nick) {
			return true;
		}
	}
	return false;
}

bool Server::isExistChannel(const std::string &name) {
	std::map<std::string, Channel *>::iterator ite = this->_channels.end();
	for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != ite; ++it) {
		if (it->first == name) {
			return true;
		}
	}
	return false;
}