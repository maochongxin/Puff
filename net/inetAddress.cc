#include "Redis/net/InetAddress.h"

using namespace redis;
using namespace redis::net;

InetAddress::InetAddress(uint16_t port = 0) {
	bzero(&_addr, sizeof(_addr));
	_addr.sin_family = AF_INET;

}

//
InetAddress(std::string ip, uint16_t port);

std::string toIp() const;
std::string toIpPort() const;
uint16_t toPort() const;

uint32_t ipNetEndian() const;
