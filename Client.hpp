#pragma once

class Client {
	private:
		char buf_read[512];
		
		char buf_write[512]; 

	public:
		Client();
		~Client();
};
