/*
 * \brief  Generic IPC infrastructure
 * \author Norman Feske
 * \date   2009-10-02
 *
 * This file is used for platforms that only use the generic IPC API. A platform
 * may extend the generic API with platform-specific marshalling operators by
 * providing a custom version of 'ipc.h' in its 'base-<platform>' repository.
 */

/*
 * Copyright (C) 2009-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__IPC_H_
#define _INCLUDE__BASE__IPC_H_

#include <base/ipc_generic.h>

/**
 * Marshalling of capabilities as plain data representation
 */
inline void
Genode::Ipc_ostream::_marshal_capability(Genode::Native_capability const &cap)
{
	_write_to_buf(cap);
}

/**
 * Unmarshalling of capabilities as plain data representation
 */
inline void
Genode::Ipc_istream::_unmarshal_capability(Genode::Native_capability &cap)
{
	_read_from_buf(cap);
}

#endif /* _INCLUDE__BASE__IPC_H_ */
