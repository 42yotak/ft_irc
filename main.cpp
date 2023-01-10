#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Utils.hpp"

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cout << RED "./ircserv <port> <password>" NC "\n";
	}

	Server *serv = callServer(std::string(argv[1]), std::string(argv[2]));

	try {
		serv->on();
	} catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	serv->off();

	return 0;
}
