#pragma once

#include <vector>
#include <string>

#define RED "\e[0;91m"
#define GREEN "\e[0;92m"
#define YELLOW "\e[0;93m"
#define BLUE "\e[0;94m"
#define PURPLE "\e[0;95m"
#define NC "\e[0;97m"

#define SERVER_NAME "PokémonGo"

class Command;

Command *callCommand();

std::vector<std::string>	split(std::string str, std::string delimiter);
