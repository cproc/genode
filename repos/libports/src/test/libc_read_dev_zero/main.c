#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd = open("/dev/zero", O_RDONLY);

	char buf[1024];
	ssize_t read_result = read(fd, buf, sizeof(buf));

	printf("read result: %zd, errno: %d\n", read_result, errno);

	return 0;
}
