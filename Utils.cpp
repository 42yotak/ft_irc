#include "Utils.hpp"
#include "Command.hpp"
#include "Server.hpp"

Command *callCommand() {
	static Command *command;

	if (command == 0) {
		command = new Command();
	}
	return command;
}

void print_split(std::vector<std::string> str) {
	std::vector<std::string>::iterator it = str.begin();
	std::vector<std::string>::iterator ite = str.end();
	int i = 1;

	while (it != ite) {
		std::cout << i << (*it) << std::endl;
		++it;
		++i;
	}
}

std::vector<std::string>	split(std::string str, std::string delimiter) {
	std::vector<std::string> ret;
	size_t pos = 0;
	while ((pos = str.find(delimiter)) != std::string::npos) {
		if (str[0] == ':') {
			ret.push_back(str.substr(1, std::string::npos));
			str.clear();
			break;
		}
		ret.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	if (!str.empty()) {
		ret.push_back(str);
		str.clear();
	}
	// print_split(ret);
	return (ret);
}
