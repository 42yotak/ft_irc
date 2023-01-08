#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

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
	struct sockaddr_in servAdr;
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	this->_servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	// bool option = true;
	// std::cout << "sockopt " << setsockopt( _servSock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option) ) << std::endl;
	std::cout << "this.servsock fron socket(): " << this->_servSock << std::endl;
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	std::cout << "port : " << atoi(port.c_str()) << std::endl;
	servAdr.sin_port = htons(atoi(port.c_str()));

	try {
		if (bind(this->_servSock, (struct sockaddr*)&servAdr, sizeof(servAdr)) == ERROR)
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
						FD_SET(clientFd, &this->_readFds);
						FD_SET(clientFd, &this->_writeFds);
						Client newClient;
						this->_clients.insert(std::make_pair(clientFd, newClient));
						// ? welcome protocol in here?
						// this->servRecv(this->_servSock);
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

			// send to client
			if (FD_ISSET(fd, &cpWriteFds)) {
				// this->servSend(fd);
				std::cout << "write!!" << std::endl;
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

	try {
		nbytes = recv(fd, (void *)buf, 512, MSG_DONTWAIT);
		std::cout << "nbytes: " << nbytes << std::endl;
		if (nbytes > 512 || nbytes == ERROR) {
			throw(std::runtime_error("recv message "));
			// TODO : close fd, FD_CLR
		}

		// close request
		if (nbytes == 0) {
			FD_CLR(fd, &this->_readFds);
			FD_CLR(fd, &this->_writeFds);
			close(fd);
		}
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
	}
	buf[nbytes] = '\0';
	std::string recvMsg(buf);
	std::cout << recvMsg << std::endl;
}
