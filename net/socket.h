#ifndef _REDIS_NET_SOCKET_H_
#define _REDIS_NET_SOCKET_H_

namespace redis {

namespace net {

class Socket {
public:
	explicit Socket(int sockfd): _sockfd(sockfd) {}
	Socket(const Socket&) = delete;
	Socket& operator= (const Socket&) = delete; 
	~Socket();

public:
	inline int fd() const { return _sockfd; }
	void bind(const InetAddress& addr);
	void listen();
	int accept(InetAddress& addr);
private:
	int _sockfd;
}

} // namespace net

} // namespace redis



#endif
