#include <stdio.h>
#include <sys/select.h>
#include <termios.h>

int main()
{
	fd_set readfds, writefds, exceptfds;

	FD_ZERO(&readfds);
	FD_SET(0, &readfds);

	struct termios tty;

        tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP
                          |INLCR|IGNCR|ICRNL|IXON);
        tty.c_oflag |= OPOST;
        tty.c_lflag &= ~(ECHO|ECHONL|ICANON|IEXTEN);
        tty.c_cflag &= ~(CSIZE|PARENB);
        tty.c_cflag |= CS8;
        tty.c_cc[VMIN] = 1;
        tty.c_cc[VTIME] = 0;

	tcsetattr (0, TCSANOW, &tty);

	for(;;) {
		printf("calling select()\n");
		int ret = select(1, &readfds, 0, 0, 0);
                printf("stdin ready for reading\n");
		char c;
		if(read(0, &c, 1) == 1)
			printf("read: %u\n", c);
		else
			printf("read error\n");
	}

	return 0;
}
