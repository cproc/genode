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
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ifaddrs.h>
#include <pthread.h>


static int argc;
static char **argv;


static void test_getnameinfo()
{
	struct ifaddrs *addrs = NULL;

	if (getifaddrs(&addrs)) {
		printf("Check getifaddrs failed\n");
		exit(~0);
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
		exit(~0);
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


static void test_getaddrinfo()
{
	struct addrinfo hints;
	char ipstr[INET6_ADDRSTRLEN];

	int i;
	for (i = 1; i < argc; ++i) {
		int res;
		char const *arg = argv[i];

		struct addrinfo *info, *p;

		memset(&hints, 0x00, sizeof(hints));
		hints.ai_family = AF_UNSPEC;

		res = getaddrinfo(arg, NULL, &hints, &info);
		if (res != 0) {
			printf("getaddrinfo error: %d\n", res);
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
			printf("%s: %s\n", arg, ipstr);

			break;
		}

		freeaddrinfo(info);
	}
}


void *test_getaddrinfo_func(void *arg)
{
	for (int i = 0; i < 100; i++)
		test_getaddrinfo();

	return NULL;
}


int main(int main_argc, char **main_argv)
{
	argc = main_argc;
	argv = main_argv;

	//test_getnameinfo();
	//test_getaddrinfo();

	enum { NUM_THREADS = 2 };

	pthread_t threads[NUM_THREADS];

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_create(&threads[i], NULL, test_getaddrinfo_func, NULL);

	for (int i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	return 0;
}
