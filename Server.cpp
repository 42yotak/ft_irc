#include "Server.hpp"

Server::Server(std::string port, std::string password) : _port(port), _password(password) {
	this->_servSock = 0;

	FD_ZERO(&this->_readFds);
	FD_ZERO(&this->_writeFds);
}

Server::~Server() {
	this->_clients.clear();
}

void Server::on(std::string port, std::string password) {
	this->_servSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&this->_servAdr, 0, sizeof(_servAdr));
	this->_servAdr.sin_family = AF_INET;
	this->_servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_servAdr.sin_port = atoi(port.c_str());

	try {
		if (bind(this->_servSock, (struct sockaddr*)&this->_servAdr, sizeof(_servAdr)) == ERROR)
			throw std::runtime_error("bind ");
		if (listen(_servSock, 42) == ERROR)
			throw std::runtime_error("listen ");
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	FD_SET(_servSock, &_readFds);

	fd_set	cpReadFds;
	fd_set	cpWriteFds;
	int	selectFds;
	int	maxFd;

	// for accept()
	struct sockaddr_in	clientAdr;
	int									clientLen, clientFd = 0;

	while(42) {
		FD_COPY(&this->_readFds, &cpReadFds);
		FD_COPY(&this->_writeFds, &cpWriteFds);

		try {
			if ((selectFds = select(maxFd, &cpReadFds, &cpWriteFds, NULL, NULL)) == ERROR)
				throw std::runtime_error("select ");
		} catch(std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
		if (selectFds == 0)
			continue ;
		// accept, maxFd처리
		for (int fd = 0; fd < maxFd + 1; fd++) {
			if (FD_ISSET(fd, &cpReadFds)) {
				if (fd == this->_servSock) { // accept call
					try {
						clientFd = accept(fd, (struct sockaddr *)&clientAdr, (socklen_t *)&clientLen);
						if (clientFd < 0) {
							throw std::runtime_error("accept ");
						}
						FD_SET(clientFd, &this->_readFds);
						FD_SET(clientFd, &this->_writeFds);
						if (clientFd > maxFd) {
							maxFd = clientFd;
						}
					} catch(std::exception &e) {
						std::cerr << e.what() << std::endl;
					}
				} else {

				}
			}
			if (FD_ISSET(fd, &cpWriteFds)) {

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
