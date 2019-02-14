/*
 * \brief  Libc tcp send and recv test
 * \author Emery Hemingway
 * \date   2018-09-17
 */

/*
 * Copyright (C) 2018 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Libc includes */
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

/* PCG includes */
//#include <pcg_variants.h>

enum { NUM_TEST_INTS = 1<<20, BULK_ITERATIONS = 1<<8 };

static uint32_t data[NUM_TEST_INTS];

int test_send(char const *host)
{
	usleep(1000000);

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("`socket` failed");
		return sock;
	}

	{
		struct sockaddr_in addr;
		addr.sin_family      = AF_INET;
		addr.sin_addr.s_addr = inet_addr(host);
		addr.sin_port        = htons(2222);

		fprintf(stderr, "connect to %s\n", host);
		if (connect(sock, (struct sockaddr *)&addr, sizeof(addr))) {
			perror("`connect` failed");
			return ~0;
		}
	}

	//pcg32_random_t rng = PCG32_INITIALIZER;
	for (int j = 0; j < BULK_ITERATIONS; ++j) {
		for (size_t i = 0; i < NUM_TEST_INTS; ++i) {
			data[i] = i/*pcg32_random_r(&rng)*/;
			//fprintf(stderr, "d[%zu]: %08x\n", i, data[i]);
		}

		size_t total = sizeof(data);
		size_t offset = 0;

		char *buf = (char *)data;
		while (offset < total) {
			//fprintf(stderr, "s1\n");
			ssize_t res = send(sock, buf+offset, total-offset, 0);
			//fprintf(stderr, "s2: %zu, %zu\n", res, offset + res);
			if (res < 1) {
				perror("send failed");
				return -1;
			}
			offset += res;
		}
	}

	fprintf(stderr, "close server\n");
	fprintf(stderr, "calling shutdown()\n");
	shutdown(sock, SHUT_RDWR);
	fprintf(stderr, "calling usleep()\n");
	usleep(40000000);
fprintf(stderr, "returning 0\n");
	return 0;
}

int test_recv()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("`socket` failed");
		return sock;
	}

	{
		struct sockaddr_in addr;
		addr.sin_family      = AF_INET;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port        = htons(2222);

		if (bind(sock, (struct sockaddr *)&addr, sizeof(addr))) {
			perror("`bind` failed");
			return ~0;
		}
	}

	if (listen(sock, 1)) {
		perror("listen broke");
	}

	for (;;) {
		char string[INET_ADDRSTRLEN];
		struct sockaddr_in addr;
		socklen_t super_socket_safety = sizeof(addr);

		int client = accept(sock, (struct sockaddr*)&addr, &super_socket_safety);
		if (client < 0) {
			perror("invalid socket from accept!");
			return ~0;
		}

		inet_ntop(AF_INET, &(addr.sin_addr), string, super_socket_safety);
		fprintf(stderr, "recv from %s\n", string);

		//pcg32_random_t rng = PCG32_INITIALIZER;
		for (int j = 0; j < BULK_ITERATIONS; ++j) {

fprintf(stderr, "*** bulk iteration %d\n", j);

			memset(data, 0, sizeof(data));

			size_t total = sizeof(data);
			size_t offset = 0;
			char *buf = (char *)data;
			while (offset < total) {
				//fprintf(stderr, "r1\n");
				ssize_t res = recv(client, buf+offset, total-offset, 0);
				//fprintf(stderr, "r2: %zu, %zu\n", res, offset + res);
				if (res < 1) {
					perror("recv failed");
					return ~0;
				}
				offset += res;
			}

			//fprintf(stderr, "recv loop finished\n");

			for (size_t i = 0; i < NUM_TEST_INTS; ++i) {
				uint32_t rnd = i/*pcg32_random_r(&rng)*/;
				//fprintf(stderr, "d[%zu}: %08x, r: %08x\n", i, data[i], rnd);
				if (data[i] != rnd) {
					fprintf(stderr, "%d: bad data at byte offset %ld, expected %u, got %u\n", j, i<<2, rnd, data[i]);
					return ~0;
				}
			}
		}

		fprintf(stderr, "close client\n");
		shutdown(client, SHUT_RDWR);
fprintf(stderr, "client/receiver returning 0\n");
		return 0;
	}
}

int main(int argc, char **argv)
{
	if (argc < 1) {
		fprintf(stderr, "no test name passed thru argv\n");
		return ~0;
	}
	if (argc < 2) {
		if (strcmp(argv[0], "./recv") == 0)
			return test_recv();
	}
	if (argc < 3) {
		if (strcmp(argv[0], "./send") == 0)
			return test_send(argv[1]);
	}

	fprintf(stderr, "\"%s\" not a valid test\n", argv[0]);
	return ~0;
}
