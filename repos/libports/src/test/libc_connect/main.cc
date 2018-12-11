#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>


static char const *server_connected           = "94.130.141.228"; /* genode.org */
static char const *server_connection_refused  = "10.0.2.3";//"10.0.2.255";
static char const *server_timeout             = "10.0.2.1";
static int const port = 80;


static void test_blocking_connect_connected()
{
	printf("Testing blocking connect (connected)\n");

	int s = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_connected);

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int res = connect(s, paddr, sizeof(addr));

	if (res != 0) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	close(s);
}


static void test_blocking_connect_connection_refused()
{
	printf("Testing blocking connect (connection refused)\n");

	int s = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_connection_refused);

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int res = connect(s, paddr, sizeof(addr));

	if (!((res == -1) && (errno == ECONNREFUSED))) {
printf("res: %d, errno: %d\n", res, errno);
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	close(s);
}


static void test_blocking_connect_timeout()
{
	printf("Testing blocking connect (timeout)\n");

	int s = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_timeout);

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int res = connect(s, paddr, sizeof(addr));

	if (!((res == -1) && (errno == ETIMEDOUT))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	close(s);
}


static void test_nonblocking_connect_connected()
{
	printf("Testing nonblocking connect (connected)\n");

	int s = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(s, F_SETFL, O_NONBLOCK);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_connected);

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int res = connect(s, paddr, sizeof(addr));

	if (!((res == -1) && (errno == EINPROGRESS))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	/* wait until socket is ready for writing */

	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(s, &writefds);

	struct timeval timeout {10, 0};
	res = select(s + 1, NULL, &writefds, NULL, &timeout);

	if (res != 1) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	int so_error = 0;
	socklen_t opt_len = sizeof(so_error);

	res = getsockopt(s, SOL_SOCKET, SO_ERROR, &so_error, &opt_len);

	if (!((res == 0) && (so_error == 0))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	res = getsockopt(s, SOL_SOCKET, SO_ERROR, &so_error, &opt_len);

	if (!((res == 0) && (so_error == 0))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	res = connect(s, paddr, sizeof(addr));

	if (!((res == 0) || ((res == -1) && (errno == EISCONN)))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	res = connect(s, paddr, sizeof(addr));

	if (!((res == -1) && (errno == EISCONN))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	close(s);
}


static void test_nonblocking_connect_connection_refused()
{
	printf("Testing nonblocking connect (connection refused)\n");

	int s = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(s, F_SETFL, O_NONBLOCK);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_connection_refused);

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int res = connect(s, paddr, sizeof(addr));

	if (!((res == -1) && (errno == EINPROGRESS))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	/* wait until socket is ready for writing */

	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(s, &writefds);

	struct timeval timeout {10, 0};
	res = select(s + 1, NULL, &writefds, NULL, &timeout);
printf("res: %d, errno: %d\n", res, errno);
	if (res != 1) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	int so_error = 0;
	socklen_t opt_len = sizeof(so_error);

	res = getsockopt(s, SOL_SOCKET, SO_ERROR, &so_error, &opt_len);
printf("res: %d, so_error: %d\n", res, so_error);
	if (!((res == 0) && (so_error == ECONNREFUSED))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	res = getsockopt(s, SOL_SOCKET, SO_ERROR, &so_error, &opt_len);

	if (!((res == 0) && (so_error == 0))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	res = connect(s, paddr, sizeof(addr));

	if (!((res == -1) && (errno == ECONNABORTED))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	close(s);
}


static void test_nonblocking_connect_timeout()
{
	printf("Testing nonblocking connect (timeout)\n");

	int s = socket(AF_INET, SOCK_STREAM, 0);

	fcntl(s, F_SETFL, O_NONBLOCK);

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server_timeout);

	sockaddr const *paddr = reinterpret_cast<sockaddr const *>(&addr);

	int res = connect(s, paddr, sizeof(addr));

	if (!((res == -1) && (errno == EINPROGRESS))) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	/* wait until socket is ready for writing */

	fd_set writefds;
	FD_ZERO(&writefds);
	FD_SET(s, &writefds);

	struct timeval timeout {10, 0};
	res = select(s + 1, NULL, &writefds, NULL, &timeout);

	if (res != 0) {
		printf("error: '%s' failed\n", __func__);
		exit(-1);
	}

	close(s);
}


int main(int argc, char *argv[])
{
	//test_blocking_connect_connected();
	test_blocking_connect_connection_refused();
	//test_blocking_connect_timeout();

	//test_nonblocking_connect_connected();
	test_nonblocking_connect_connection_refused();
	//test_nonblocking_connect_timeout();

	return 0;
}
