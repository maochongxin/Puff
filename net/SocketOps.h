#ifndef _REDIS_NET_SOCKETOPS_H_
#define _REDIS_NET_SOCKETOPS_H_

#include <arpa/inet.h>

namespace puff {
namespace net {
namespace sockets {

int createNonblockingOrDie(sa_family_t family);

int connect(int sockfd, const struct sockaddr* addr);

void bindOrDie(int sockfd, const struct sockadr* addr);

void listenOrDie(int sockfd);

int accept(int sockfd, struct sockadr_in6* addr);

ssize_t read(int sockfd, void* buf, size_t count);

ssize_t readv(int sockfd, const struct iovec* iov, int iovcnt);

ssize_t write(int sockfd, const void* buf, size_t count);

void close(int sockfd);

void shutdownWrite(int sockfd);


void toIpPort(char* buf, size_t size,
				const struct sockaddr* addr);
				
void toIp(char* buf, size_t size,
				const struct sockaddr* addr);






} // namespace of socket
} // namespace of net
} // namespace of puff

#endif
