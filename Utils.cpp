#include "Utils.hpp"

Command *callCommand() {
	static Command *command;

	if (command == 0) {
		command = new Command();
	}
	return command;
}

std::vector<std::string>	split(std::string str, std::string delimiter) {
	std::vector<std::string> ret;
	size_t pos = 0;
	// CAP LS\r\nPASS \r\nNICK \r\nUSER \r\n
	while ((pos = str.find(delimiter)) != std::string::npos) {
		ret.push_back(str.substr(0, pos + delimiter.length()));
		str.erase(0, pos + delimiter.length());
	}
	return (ret);
}