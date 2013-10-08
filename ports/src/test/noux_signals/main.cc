#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void signal_handler(int signal, siginfo_t *siginfo, void *context)
{
	printf("%d: signal handler for signal %d, sender = %d, code = %d, %d, %d\n",
	       getpid(),
	       signal,
	       siginfo->si_pid,
	       siginfo->si_code,
		   SI_USER, SI_KERNEL
		   );
}

int main(int argc, char *argv[])
{
	char c;

	struct sigaction sa;

	memset (&sa, '\0', sizeof(sa));

	sa.sa_sigaction = signal_handler;
	sa.sa_flags = SA_SIGINFO;

	sigaction(SIGINT, &sa, 0);

	if (fork() > 0)
		printf("%d: parent\n", getpid());
	else
		printf("%d: child\n", getpid());

	for(;;) {
		if ((read(0, &c, 1) == -1) && (errno = EINTR))
			printf("%d: EINTR\n", getpid());
		else
			printf("%d: c = %x\n", getpid(), c);
	}

	return 0;
}

