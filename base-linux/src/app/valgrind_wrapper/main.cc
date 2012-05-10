/*
 * \brief  Valgrind wrapper
 * \author Christian Prochaska
 * \date   2012-05-10
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <stdio.h>
#include <unistd.h>

extern char **genode_argv;

int main(int argc, char *argv[])
{
	execvp("valgrind", genode_argv);
	perror("execvp() failed");
	return -1;
}
