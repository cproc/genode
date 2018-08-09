#include <fcntl.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int s = socket(AF_INET, SOCK_STREAM, 0);

	printf("socket() returned %d\n", s);

	fcntl(s, F_SETFL, O_NONBLOCK);

	char const *server = "94.130.141.228"; /* genode.org */
	int port = 81;

	sockaddr_in const addr { 0, AF_INET, htons(port), { inet_addr(server) } };

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int ret = connect(s, paddr, sizeof(addr));

	printf("connect() returned %d\n", ret);

	return 0;
}
