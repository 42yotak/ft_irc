#pragma once

class Command {
	public:
	// 냅다 void return하기
	// 구현할 함수에 대한 설명 참고! https://irssi.org/documentation/help/
	// 같은 이름의 다른 함수가 있을까봐 irc붙임
		void ircPass();
		void ircNick();
		void ircUser();
		void ircPing();
		void ircJoin();
		void ircPart(); // 현재 client가 위치한 채널 정보 필요
		void ircPrivmsg();
		void ircNotice();
		void ircQuit();
		void ircKick();
};