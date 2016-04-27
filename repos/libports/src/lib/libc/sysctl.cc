/*
 * \brief  Sysctl facade
 * \author Emery Hemingway
 * \date   2016-04-27
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <util/string.h>
#include <base/env.h>

/* Genode-specific libc interfaces */
#include <libc-plugin/plugin.h>

/* Libc includes */
#include <sys/sysctl.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "libc_errno.h"


enum { PAGESIZE = 4096 };


extern "C" long sysconf(int name)
{
	switch (name) {
	case _SC_PAGESIZE: return PAGESIZE;
	case _SC_PHYS_PAGES:
		return Genode::env()->ram_session()->quota() / PAGESIZE;
	default:
		PWRN("unhandled request for sysconf code %d", name);
		return -1;
	}
}


extern "C" int __sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp,
                        void *newp, size_t newlen)
{
	static const ctlname ctl_names[] = CTL_NAMES;
	static const ctlname ctl_kern_names[] = CTL_KERN_NAMES;
	static const ctlname ctl_hw_names[] = CTL_HW_NAMES;
	static const ctlname ctl_user_names[] = CTL_USER_NAMES;
	static const ctlname ctl_p1003_1b_names[] = CTL_P1003_1B_NAMES;

	/* read only */
	if (!oldp || (*oldlenp < sizeof(int))) {
		/* check for write attempt */
		errno = newp ? EPERM : EINVAL;
		return -1;
	}

	Libc::Absolute_path sysctl_path("/.sysctl/");

	ctlname const *ctl = nullptr;
	char *buf = (char*)oldp;
	int index_a = name[0];
	int index_b = name[1];
	int fd;

	if (namelen != 2) return Libc::Errno(ENOENT);
	if (index_a >= CTL_MAXID) return Libc::Errno(EINVAL);

	sysctl_path.append(ctl_names[index_a].ctl_name);
	sysctl_path.append("/");

	switch(index_a) {
	case CTL_KERN:
		if (index_b >= KERN_MAXID) return Libc::Errno(EINVAL);
		ctl = &ctl_kern_names[index_b]; break;

	case CTL_HW:
		if (index_b >= HW_MAXID) return Libc::Errno(EINVAL);
		ctl = &ctl_hw_names[index_b]; break;

	case CTL_USER:
		if (index_b >= USER_MAXID) return Libc::Errno(EINVAL);
		ctl = &ctl_user_names[index_b]; break;

	case CTL_P1003_1B:
		if (index_b >= CTL_P1003_1B_MAXID) return Libc::Errno(EINVAL);
		ctl = &ctl_p1003_1b_names[index_b]; break;
	}
	if (!ctl) return Libc::Errno(EINVAL);

	sysctl_path.append(ctl->ctl_name);

	/*
	 * read from /.sysctl/...
	 *
	 * The abstracted libc interface is used to read files here
	 * rather than to explicity resolve a file system plugin.
	 */
	fd = open(sysctl_path.base(), 0);
	if (fd != -1) {

		auto n = read(fd, buf, *oldlenp);
		close(fd);

		if (n > 0) switch (ctl->ctl_type) {
		case CTLTYPE_INT: {
			long value = 0;
			Genode::ascii_to((char*)oldp, value);
			*(int*)oldp = int(value);
			*oldlenp = sizeof(int);
			return 0;
		}

		case CTLTYPE_STRING:
			*oldlenp = n;
			return 0;

		default:
			PERR("unhandle sysctl data type for %s", sysctl_path.base());
			return Libc::Errno(EINVAL);
		}

	}
	/* fallthru to some baked-in defaults */
	{
		switch(index_a) {

		case CTL_KERN:
			switch(index_b) {
			case KERN_OSTYPE:
				Genode::strncpy(buf, "Genode", *oldlenp);
				*oldlenp = Genode::strlen(buf);
				return 0;

			case KERN_OSRELEASE:
				Genode::strncpy(buf, GENODE_OSRELEASE, *oldlenp);
				*oldlenp = Genode::strlen(buf);
				return 0;

			case KERN_OSREV:
				*(int*)oldp = int(GENODE_OSREV);
				*oldlenp = sizeof(int);
				return 0;

			case KERN_VERSION:
				Genode::strncpy(buf, GENODE_VERSION, *oldlenp);
				*oldlenp = Genode::strlen(buf);
				return 0;

			case KERN_HOSTNAME:
				*buf = '\0';
				*oldlenp = 1;
				return 0;

			} break;

		case CTL_HW: switch(index_b) {

			case HW_MACHINE:
				Genode::strncpy(buf, GENODE_MACHINE, *oldlenp);
				*oldlenp = Genode::strlen(buf);
				return 0;

			case HW_NCPU:
				*(int*)oldp = 1;
				*oldlenp = sizeof(int);
				return 0;

			case HW_PHYSMEM:
				*(unsigned long*)oldp = Genode::env()->ram_session()->quota();
				*oldlenp = sizeof(unsigned long);
				return 0;

			case HW_PAGESIZE:
				*(int*)oldp = PAGESIZE;
				*oldlenp = sizeof(int);
				return 0;

			case HW_MODEL:
			case HW_MACHINE_ARCH:
				Genode::strncpy(buf, "unknown", *oldlenp);
				*oldlenp = Genode::strlen(buf);
				return 0;

			} break;

		case CTL_P1003_1B: switch(index_b) {

			case CTL_P1003_1B_PAGESIZE:
				*(int*)oldp = 4096;
				*oldlenp = sizeof(int);
				return 0;

			} break;

		}
	}

	PWRN("sysctl %s not found", sysctl_path.base());
	return Libc::Errno(ENOENT);
}

