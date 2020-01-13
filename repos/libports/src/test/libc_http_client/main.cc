#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

static constexpr bool verbose = true;
static constexpr bool verbose_response = true;
static constexpr bool multithreaded = false;

static constexpr char const *server = "94.130.141.228"; /* genode.org */
static constexpr int port = 80;
static constexpr ssize_t expected_response_size = 10975;

static struct sockaddr_in addr;
static sockaddr const *paddr;

void test(int thread)
{
	for (int i = 1; i <= 1; i++) {

		if (verbose)
			printf("[%d] i: %d\n", thread, i);

		int s = socket(AF_INET, SOCK_STREAM, 0);

		if (verbose)
			printf("[%d] socket() returned %d\n", thread, s);

		int ret = connect(s, paddr, sizeof(addr));

		if (verbose)
			printf("[%d] connect() returned %d\n", thread, ret);

		if (ret != 0) {
			printf("[%d] Error: connect() failed, errno: %d\n", thread, errno);
			exit(-1);
		}

		char const *request = "GET / HTTP/1.0\r\n\r\n";

		ssize_t bytes_written = write(s, request, strlen(request));

		if (verbose)
			printf("[%d] write() returned %zd\n", thread, bytes_written);


		/* peek */

		for (int j = 0; j < 2; j++) {

			char response[64*1024] = { 0 };

			ssize_t bytes_peeked = 0;
		
			do {
				bytes_peeked = recv(s, response, sizeof(response), MSG_PEEK);

				if (verbose)
					printf("[%d] recv() returned %zd\n", thread, bytes_peeked);

			} while (bytes_peeked < expected_response_size);

			if (verbose)
				printf("[%d] bytes peeked: %zd\n", thread, bytes_peeked);

			if (bytes_peeked != expected_response_size) {
				printf("[%d] Error: recv size mismatch\n", thread);
				exit(-1);
			}

			if (verbose_response)
				printf("[%d] response:\n%s\n", thread, response);
		}


		/* read */

		{
			char response[64*1024] = { 0 };

			ssize_t bytes_read_total = 0;
			ssize_t bytes_read = 0;
			
			do {
				bytes_read = read(s, &response[bytes_read_total], sizeof(response) - bytes_read_total);

				if (verbose)
					printf("[%d] read() returned %zd\n", thread, bytes_read);

				bytes_read_total += bytes_read;
			} while (bytes_read > 0);

			if (verbose)
				printf("[%d] bytes read: %zd\n", thread, bytes_read_total);

			if (bytes_read_total != expected_response_size) {
				printf("[%d] Error: read size mismatch\n", thread);
				exit(-1);
			}

			if (verbose_response)
				printf("[%d] response:\n%s\n", thread, response);
		}


		/* peek again */

		{
			char response[64*1024] = { 0 };

			ssize_t bytes_peeked = recv(s, response, sizeof(response), MSG_PEEK);

			if (verbose)
				printf("[%d] recv() returned %zd\n", thread, bytes_peeked);

			if (bytes_peeked != 0)
				printf("[%d] Error: peek did not return 0\n", thread);
		}

		close(s);
	}
}


void *thread_func(void *arg)
{
	test(*(int*)arg);
	return 0;
}


int main(int argc, char *argv[])
{
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(server);

	paddr = reinterpret_cast<sockaddr const *>(&addr);

	if (multithreaded) {

		pthread_t t1;
		int arg1 = 1;
		pthread_create(&t1, 0, thread_func, &arg1);

		pthread_t t2;
		int arg2 = 2;
		pthread_create(&t2, 0, thread_func, &arg2);

		pthread_join(t1, 0);
		pthread_join(t2, 0);

	} else {
		test(0);
	}

	return 0;
}
