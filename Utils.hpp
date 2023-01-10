#pragma once

#include <vector>
#include <string>

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define NC "\e[0;37m"

class Command;
class Server;

Command *callCommand();

Server *callServer(std::string port="6667", std::string password="0000");

std::vector<std::string>	split(std::string str, std::string delimiter);