#pragma once

#include "Protocol.hpp"

class Command {
	private:
		Protocol _protocol;
	public:
		void ircPass();
		void ircNick();
		void ircUser();
		void ircPing();
		void ircJoin();
		void ircPart();
		void ircPrivmsg();
		void ircNotice();
		void ircQuit();
		void ircKick();
};