#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>
#include <iostream>

namespace net {
	class TcpSocket {
	public:
		TcpSocket(int domain, int port, std::string addr = "0.0.0.0");
		~TcpSocket();

	public:
		bool bind(const std::string& err_code);
		bool listen(const std::string& err_code, int backlog = 128);
		int  accept();
		bool connect(const std::string& err_code);
		int  fd() const;
		const struct sockaddr_in& get sock_addr()const;
	private:
		int listen_fd; // listen socket
		struct sockaddr_in server_addr;
	};

}; // end of namespace
