#include "Server.hpp"
#include "Client.hpp"

Server::Server(std::string port, std::string password) : _port(port), _password(password) {
	this->_servSock = 0;

	FD_ZERO(&this->_readFds);
	FD_ZERO(&this->_writeFds);
}

Server::~Server() {
	// this->_clients.clear();
}

void Server::on(std::string port, std::string password) {
	(void) password;
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 5000;
	
	this->_servSock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&this->_servAdr, 0, sizeof(_servAdr));
	this->_servAdr.sin_family = AF_INET;
	this->_servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->_servAdr.sin_port = htons(atoi(port.c_str()));

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
	int	selectFds = 0;
	int	maxFd = 0;

	// for accept()
	struct sockaddr_in	clientAdr;
	int									clientLen, clientFd = 0;

	while(42) {
		FD_COPY(&this->_readFds, &cpReadFds);
		FD_COPY(&this->_writeFds, &cpWriteFds);

		try {
			if ((selectFds = select(maxFd, &cpReadFds, &cpWriteFds, NULL, &timeout)) == ERROR)
				throw std::runtime_error("select ");
		} catch(std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
		if (selectFds == 0)
			continue ;
		for (int fd = 0; fd < maxFd + 1; fd++) {
			if (FD_ISSET(fd, &cpReadFds)) { // revc from client
				if (fd == this->_servSock) { // accept call
					try {
						clientFd = accept(fd, (struct sockaddr *)&clientAdr, (socklen_t *)&clientLen);
						if (clientFd < 0) {
							throw std::runtime_error("accept ");
						}
						FD_SET(clientFd, &this->_readFds);
						FD_SET(clientFd, &this->_writeFds);
						// this->_clients.insert(std::make_pair(clientFd, Client()));
						// ? welcome protocol in here?
						this->servRecv(this->_servSock);
						if (clientFd > maxFd) {
							maxFd = clientFd;
						}
					} catch(std::exception &e) {
						std::cerr << e.what() << std::endl;
					}
				} else {
					// TODO: read message
					this->servRecv(fd);
				}
			}
			if (FD_ISSET(fd, &cpWriteFds)) { // send to client
				// this->servSend(fd);
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

void Server::servRecv(int fd) {
	ssize_t	nbytes;
	char buf[512];
	nbytes = recv(fd, buf, sizeof(buf), MSG_PEEK | MSG_DONTWAIT);

	try {
		if (nbytes > 512 || nbytes == ERROR) {
			throw(std::runtime_error("recv message "));
			// TODO : close fd, FD_CLR
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	std::string recvMsg = buf;
	std::cout << recvMsg << std::endl;
}