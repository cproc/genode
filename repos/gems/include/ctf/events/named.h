/*
 * \brief  Ctf trace event for logging names
 * \author Johannes Schlatow
 * \date   2021-08-03
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CTF__EVENTS__NAMED_H_
#define _CTF__EVENTS__NAMED_H_

#include <util/string.h>

namespace Ctf {
	struct Named_event;
}

/**
 * Base type for CTF trace events with a string as payload.
 */
struct Ctf::Named_event
{
	char _name[0];

	Named_event(const char *name, size_t len) {
		Genode::copy_cstring(_name, name, len);
	}

	Named_event(const char *name1, size_t len1,
	            const char *name2, size_t len2) {
		Genode::copy_cstring(_name, name1, len1);
		Genode::copy_cstring(&_name[len1], name2, len2);
	}

} __attribute__((packed));

#endif /* _CTF__EVENTS__NAMED_H_ */
