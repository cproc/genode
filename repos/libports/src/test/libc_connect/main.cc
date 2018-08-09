#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int s = socket(AF_INET, SOCK_STREAM, 0);

	printf("socket() returned %d\n", s);

	fcntl(s, F_SETFL, O_NONBLOCK);

	char const *server = "94.130.141.228"; /* genode.org */
	int port = 80;

	sockaddr_in const addr { 0, AF_INET, htons(port), { inet_addr(server) } };

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int ret = connect(s, paddr, sizeof(addr));

	printf("connect() returned %d\n", ret);

	if (ret == -1) {
		switch (errno) {
		case EINPROGRESS:
			{
				fd_set writefds;
				FD_ZERO(&writefds);
				FD_SET(s, &writefds);
				struct timeval timeout {10, 0};
				printf("calling select()\n");
				int res = select(s + 1, NULL, &writefds, NULL, &timeout);
				printf("select() returned %d\n", res);
				if (res == 0) {
					printf("select() timed out\n");
					//return -1;
				}
				break;
			}
		default: printf("errno: %d\n", errno);
		}
	}

	ret = connect(s, paddr, sizeof(addr));


	printf("connect() returned %d\n", ret);

	return 0;
}
