#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
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
	//char const *server = "10.0.0.2";
	int port = 80;

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server);

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int ret = connect(s, paddr, sizeof(addr));

	printf("connect() returned %d\n", ret);

	if (ret == -1) {
		printf("errno: %d\n", errno);
		switch (errno) {
		case EINPROGRESS:
			{
				fd_set writefds;
				FD_ZERO(&writefds);
				FD_SET(s, &writefds);
				struct timeval timeout {100, 0};
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

	int so_error = 0;
	socklen_t opt_len = sizeof(so_error);

	ret = getsockopt(s, SOL_SOCKET, SO_ERROR, &so_error, &opt_len);

	printf("getsockopt() returned %d, %d\n", ret, so_error);

	ret = getsockopt(s, SOL_SOCKET, SO_ERROR, &so_error, &opt_len);

	printf("getsockopt() 2 returned %d, %d\n", ret, so_error);

	ret = connect(s, paddr, sizeof(addr));

	printf("second connect() returned %d\n", ret);

	if (ret < 0)
		printf("errno: %d\n", errno);

	ret = connect(s, paddr, sizeof(addr));

	printf("third connect() returned %d\n", ret);

	if (ret < 0)
		printf("errno: %d\n", errno);

	return 0;
}
