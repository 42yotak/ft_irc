#pragma once

#include <vector>

class Client;

class Channel {
	private:
		// 현재 채널에 참가한 유저 정보
		std::vector<int> userFd;
		// 방장
		int ownerFd;
		// fd만 갖고 있으므로 Server 클래스에 유저를 가져오는 함수 필요
		Client& getClient();
		// user가 나간다고 할 경우, 들어오는 경우
};