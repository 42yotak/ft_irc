#pragma once

#include <string>
class Protocol {
	public:
		// 이런식으로 프로토콜 메시지를 return 하는 클래스
		// 함수명: 기능 + proto (예시 userProto)
		std::string passProto();
};