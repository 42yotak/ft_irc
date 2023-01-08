#include "Channel.hpp"
#include "Client.hpp"
#include "Command.hpp"
#include "Protocol.hpp"
#include "Server.hpp"

int main(int argc, char *argv[]) {
	(void) argc;
	(void) argv;
	Server serv = Server("6667", "password");

	serv.on();
	serv.off();

	return 0;
}
