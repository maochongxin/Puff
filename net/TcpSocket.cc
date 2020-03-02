#include "TcpSocket.h"

namespace net {
	TcpSocket::TcpSocket(int domain, int port, std::string addr = "0.0.0.0") {
		bzero(&server_addr, sizeof(server_addr));
		server_addr.sin_family = domain;
		server_addr.sin_port = htons(port);
		server_addr.sin_addr.s_addr = inet_addr(addr.data());
	}
	bool TcpSocket::Bind(const std::string& err_code) {
		if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
			perror(err_code.data());
			return false;
		}
		return true;
	}

	bool TcpSocket::Listen(const std::string& err_code, int backlog = 128) {
		if (listen(listen_fd, backlog) < 0) {
			perror(err_code.data());
			return false;
		}
		return true;
	}

	int  TcpSocket::Accept() {
		int cnnfd = accept(listen_fd, NULL, NULL);
		return connfd;
	}

	bool Connect(const std::string& err_code) {
		if (connect(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
			perror(err_code.data());
			return false;
		}
		return true;
	}

	int  TcpSocket::GetFd() const {
		return listen_fd;
	}

	const struct sockaddr_in& get sock_addr()const {
		return server_addr;
	}

} // end of namespace
