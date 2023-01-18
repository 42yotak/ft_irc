#include "Server.hpp"
#include "Utils.hpp"
#include <signal.h>

void terminate(int sig) {
	if (sig == SIGINT)
		Server::callServer().~Server();
	exit(1);
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cout << RED "./ircserv <port> <password>" NC "\n";
		return 1;
	}
	if (atoi(argv[1]) > 65535) {
		return 1;
	}
	signal(SIGINT, terminate);
	try {
		Server::callServer(argv[1], argv[2]).on();
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	Server::callServer().off();

	return 0;
}
