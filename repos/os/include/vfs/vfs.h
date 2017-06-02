/*
 * \brief  VFS handle
 * \author Christian Prochaska
 * \date   2017-07-04
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__VFS__VFS_H_
#define _INCLUDE__VFS__VFS_H_

namespace Vfs {
	// XXX rename as Vfs_handle and Vfs_handle as File_handle
	struct Vfs_handle_base;
	struct Io_response_handler;
}


struct Vfs::Vfs_handle_base
{
	/**
	 * Opaque handle context
	 */
	struct Context { };

	Context *context = nullptr;
};

struct Vfs::Io_response_handler
{
	virtual void handle_io_response(Vfs::Vfs_handle_base::Context *context) = 0;
};

#endif /* _INCLUDE__VFS__VFS_H_ */
