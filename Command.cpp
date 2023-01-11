#include "Command.hpp"

Command::Command() {

}

Command::~Command() {

}

void Command::cmdPass(std::vector<std::string> buf_read, char *buf_write) {
// hyko
}
void Command::cmdNick(std::vector<std::string> buf_read, char *buf_write) {
// yuhwang
}
void Command::cmdUser(std::vector<std::string> buf_read, char *buf_write) {
// yotak
}
void Command::cmdPing(std::vector<std::string> buf_read, char *buf_write) {

}
void Command::cmdQuit(std::vector<std::string> buf_read, char *buf_write) {

}

void Command::cmdJoin(std::vector<std::string> buf_read, char *buf_write) {
	// 공백으로 split
	// comma로 split
	// comma로 split한 각각을 channel로
}
void Command::cmdPart(std::vector<std::string> buf_read, char *buf_write) {

}
void Command::cmdKick(std::vector<std::string> buf_read, char *buf_write) {

}

void Command::cmdNotice(std::vector<std::string> buf_read, char *buf_write) {

}
void Command::cmdPrivmsg(std::vector<std::string> buf_read, char *buf_write) {

}