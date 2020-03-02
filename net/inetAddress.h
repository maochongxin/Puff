#ifndef _REDIS_NET_INETADDRESS_H_
#define _REDIS_NET_INETADDRESS_H_

#include <string>
#include <netinet.in.h>

namespace redis {
namespace net {

class InetAddress {
public:
	//
	explicit InetAddress(uint16_t port = 0);

	//
	InetAddress(std::string ip, uint16_t port);

	explicit InetAddress(const struct sockaddr_in& addr): _addr(addr) {}

	sin_family_t family() const { return _addr.sin_family; }
	std::string toIp() const;
	std::string toIpPort() const;
	uint16_t toPort() const;

	uint32_t ipNetEndian() const;
  	uint16_t portNetEndian() const { return _addr.sin_port; }


private:
	struct sockaddr_in _addr;
};

} // namespace of net
} // namespace of redis

#endif
