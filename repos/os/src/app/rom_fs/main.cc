/*
 * \brief  Write content of ROM module to File_system
 * \author Johannes Schlatow
 * \date   2016-03-12
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <rom_session/connection.h>
#include <base/signal.h>
#include <os/config.h>
#include <os/attached_rom_dataspace.h>
#include <util/print_lines.h>
#include <os/server.h>
#include <util/volatile_object.h>

#include <base/heap.h>
#include <base/allocator_avl.h>

#include <file_system_session/connection.h>
#include <file_system/util.h>

namespace Rom_fs {
	using namespace Genode;

	struct Main;

	enum  {
		BLOCK_SIZE   = 512,
		QUEUE_SIZE   = File_system::Session::TX_QUEUE_SIZE,
		TX_BUF_SIZE  = BLOCK_SIZE * (QUEUE_SIZE*2 + 1)
	};
}


struct Rom_fs::Main
{
	Server::Entrypoint &_ep;

	Allocator_avl alloc;

	File_system::Connection _fs;

	Lazy_volatile_object<Attached_rom_dataspace> _rom_ds;

	typedef Genode::String<100> Rom_name;

	/**
	 * Name of currently requested ROM module
	 *
	 * Solely used to detect configuration changes.
	 */
	Rom_name _rom_name;

	/**
	 * Signal handler that is invoked when the configuration or the ROM module
	 * changes.
	 */
	void _handle_update(unsigned);

	Signal_rpc_member<Main> _update_dispatcher =
		{ _ep, *this, &Main::_handle_update };

	Main(Server::Entrypoint &ep) : _ep(ep), alloc(env()->heap()), _fs(alloc, TX_BUF_SIZE)
	{
		config()->sigh(_update_dispatcher);
		_handle_update(0);
	}
};


void Rom_fs::Main::_handle_update(unsigned)
{
	config()->reload();

	/*
	 * Query name of ROM module from config
	 */
	Rom_name rom_name;
	try {
		rom_name = config()->xml_node().attribute_value("rom", rom_name);

	} catch (...) {
		PWRN("could not determine ROM name from config");
		return;
	}

	/*
	 * If ROM name changed, reconstruct '_rom_ds'
	 */
	if (rom_name != _rom_name) {
		_rom_ds.construct(rom_name.string());
		_rom_ds->sigh(_update_dispatcher);
		_rom_name = rom_name;
	}

	/*
	 * Update ROM module and print content to LOG
	 */
	if (_rom_ds.is_constructed()) {
		_rom_ds->update();

		if (_rom_ds->is_valid()) {
			using namespace File_system;

			char dir_path[] = "/";
			const char *file_name = _rom_name.string();

			try {
				Dir_handle   dir_handle = ensure_dir(_fs, dir_path);
				Handle_guard dir_guard(_fs, dir_handle);
				File_handle handle;

				handle = _fs.file(dir_handle, file_name, File_system::WRITE_ONLY, true);

				_fs.truncate(handle, 0);

				size_t len     = max(strlen(_rom_ds->local_addr<char>()), _rom_ds->size());
				size_t written = write(_fs, handle, _rom_ds->local_addr<void>(), len, 0);

				if (written < len) {
					PWRN("%ld of %ld bytes have been written", written, len);
				}

				_fs.close(handle);
			} catch (Permission_denied) {
            PERR("%s%s: permission denied", dir_path, file_name);

         } catch (No_space) {
            PERR("file system out of space");

         } catch (Out_of_node_handles) {
            PERR("too many open file handles");

         } catch (Invalid_name) {
            PERR("%s%s: invalid path", dir_path, file_name);

         } catch (Name_too_long) {
            PERR("%s%s: name too long", dir_path, file_name);

         } catch (...) {
            PERR("cannot open file %s%s", dir_path, file_name);
            throw;
         }
		} else {
			PLOG("ROM '%s' is invalid", _rom_name.string());
		}
	}
}


namespace Server {

	char const *name() { return "rom_fs_ep"; }

	size_t stack_size() { return 4*1024*sizeof(long); }

	void construct(Entrypoint &ep)
	{
		static Rom_fs::Main main(ep);
	}
}
