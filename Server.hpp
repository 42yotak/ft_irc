#pragma once

#include <map>
#include "Client.hpp"

class Server {
	private:
		std::map<int, Client&> clients;
	public:
		// 네트워크 부분 처리 socket, listen, recv, 등등
		// fd로 찾은 client를 돌려주는 함수 필요
};