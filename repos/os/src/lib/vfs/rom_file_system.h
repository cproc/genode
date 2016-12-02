/*
 * \brief  ROM filesystem
 * \author Norman Feske
 * \date   2014-04-14
 */

/*
 * Copyright (C) 2014-2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__VFS__ROM_FILE_SYSTEM_H_
#define _INCLUDE__VFS__ROM_FILE_SYSTEM_H_

#include <base/attached_rom_dataspace.h>
#include <vfs/file_system.h>

namespace Vfs { class Rom_file_system; }


class Vfs::Rom_file_system : public Single_file_system
{
	private:

		struct Label
		{
			enum { LABEL_MAX_LEN = 64 };
			char string[LABEL_MAX_LEN];

			Label(Xml_node config)
			{
				/* obtain label from config */
				string[0] = 0;
				try { config.attribute("label").value(string, sizeof(string)); }
				catch (...)
				{
					/* use VFS node name if label was not provided */
					string[0] = 0;
					try { config.attribute("name").value(string, sizeof(string)); }
					catch (...) { }
				}
			}
		} _label;

		Genode::Attached_rom_dataspace _rom;

	public:

		Rom_file_system(Genode::Env &env,
		                Genode::Allocator&,
		                Genode::Xml_node config)
		:
			Single_file_system(NODE_TYPE_FILE, name(), config, OPEN_MODE_RDONLY),
			_label(config),
			_rom(env, _label.string)
		{ }

		static char const *name() { return "rom"; }


		/*********************************
		 ** Directory-service interface **
		 ********************************/

		Dataspace_capability dataspace(char const *path) override
		{
			return _rom.cap();
		}

		/*
		 * Overwrite the default open function to update the ROM dataspace
		 * each time when opening the corresponding file.
		 */
		Open_result open(char const  *path, unsigned,
		                 Vfs_handle **out_handle,
		                 Allocator   &alloc) override
		{
			Open_result const result =
				Single_file_system::open(path, 0, out_handle, alloc);

			_rom.update();

			return result;
		}


		/********************************
		 ** File I/O service interface **
		 ********************************/

		Stat_result stat(char const *path, Stat &out) override
		{
			Stat_result result = Single_file_system::stat(path, out);

			_rom.update();
			out.size = _rom.valid() ? _rom.size() : 0;

			return result;
		}


		/********************************
		 ** File I/O service interface **
		 ********************************/

		void read(Vfs_handle *vfs_handle, file_size count) override
		{
			/* file read limit is the size of the dataspace */
			file_size const max_size = _rom.size();

			/* current read offset */
			file_size const read_offset = vfs_handle->seek();

			/* maximum read offset, clamped to dataspace size */
			file_size const end_offset = min(count + read_offset, max_size);

			/* source address within the dataspace */
			char const *src = _rom.local_addr<char>() + read_offset;

			/* check if end of file is reached */
			if (read_offset >= end_offset)
				return vfs_handle->read_status(Callback::COMPLETE);

			/* pass ROM dataspace to callback */
			count = end_offset - read_offset;
			vfs_handle->read_callback(src, count, Callback::COMPLETE);
		}
};

#endif /* _INCLUDE__VFS__ROM_FILE_SYSTEM_H_ */
