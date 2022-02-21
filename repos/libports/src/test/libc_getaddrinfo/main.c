/*
 * \brief  Libc getaddrinfo(...) test
 * \author Emery Hemingway
 * \date   2018-02-18
 */

/*
 * This code lifted from Beej's Guide to Network Programming.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>

void test_getaddrinfo()
{
	struct addrinfo hints;
	char ipstr[INET6_ADDRSTRLEN];

	for (;;) {
		int res;
		char const *arg = "jitsi";

		struct addrinfo *info, *p;

		memset(&hints, 0x00, sizeof(hints));
		hints.ai_family = AF_UNSPEC;

		res = getaddrinfo(arg, NULL, &hints, &info);
		if (res != 0) {
			printf("%p: getaddrinfo error: %d\n", &res, res);
			continue;
		}

		for (p = info; p != NULL; p = p->ai_next) {
			void *addr;

			// get the pointer to the address itself,
			// different fields in IPv4 and IPv6:
			if (p->ai_family == AF_INET) { // IPv4
				struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
				addr = &(ipv4->sin_addr);
			} else { // IPv6
				struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
				addr = &(ipv6->sin6_addr);
			}

			inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
			printf("%p: %s: %s\n", &res, arg, ipstr);

			break;
		}

		freeaddrinfo(info);
	}
}

void *thread_func(void *arg)
{
	test_getaddrinfo();
}

int main(int argc, char **argv)
{
	{
		struct ifaddrs *addrs = NULL;

		if (getifaddrs(&addrs)) {
			printf("Check getifaddrs failed\n");
			return ~0;
		}

		char ip_addr[NI_MAXHOST],
		     netmask[NI_MAXHOST],
		     broadcast[NI_MAXHOST],
		     sbuf[NI_MAXSERV];

		if (getnameinfo(addrs->ifa_addr, addrs->ifa_addr->sa_len,
		                ip_addr, sizeof(ip_addr), sbuf, sizeof(sbuf),
		                NI_NUMERICHOST | NI_NUMERICSERV))
		{
			printf("could not get address from getifaddrs\n");
			return ~0;
		}

		if (getnameinfo(addrs->ifa_netmask, addrs->ifa_netmask->sa_len,
		                netmask, sizeof(netmask), sbuf, sizeof(sbuf),
		                NI_NUMERICHOST | NI_NUMERICSERV))
		{
			printf("could not get netmask from getifaddrs\n");
		}

		if (getnameinfo(addrs->ifa_broadaddr, addrs->ifa_broadaddr->sa_len,
		                broadcast, sizeof(broadcast), sbuf, sizeof(sbuf),
		                NI_NUMERICHOST | NI_NUMERICSERV))
		{
			printf("could not get broadcast from getifaddrs\n");
		}

		freeifaddrs(addrs);
		printf("getifaddrs ip_addr=%s, netmask=%s broadcast=%s\n", ip_addr, netmask, broadcast);
	}

	pthread_t t;
	pthread_create(&t, 0, thread_func, 0);

//	sleep(100);
	test_getaddrinfo();

	return 0;
}
