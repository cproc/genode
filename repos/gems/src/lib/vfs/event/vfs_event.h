/*
 * \brief  Event file system
 * \author Christian Prochaska
 * \date   2020-11-26
 */

/*
 * Copyright (C) 2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _EVENT_FILE_SYSTEM_H_
#define _EVENT_FILE_SYSTEM_H_

/* Genode includes */
#include <event_session/connection.h>
#include <vfs/single_file_system.h>


class Event_file_system : public Vfs::Single_file_system
{
	private:

		Event::Connection _event;

		class Event_vfs_handle : public Single_vfs_handle
		{
			private:
				Event::Connection &_event;

			public:

				Event_vfs_handle(Directory_service &ds,
				                 File_io_service   &fs,
				                 Genode::Allocator &alloc,
				                 Event::Connection &event)
				: Single_vfs_handle(ds, fs, alloc, 0),
				  _event(event) { }

				Read_result read(char *dst, Vfs::file_size count,
				                 Vfs::file_size &out_count) override
				{
					Genode::log("Event_vfs_handle::read()");

					_event.with_batch([&] (Event::Session_client::Batch &batch) {
						//batch.submit(Input::Press{0});
						//batch.submit(Input::Release{0});
					});

					return READ_ERR_IO;
				}

				Write_result write(char const *src, Vfs::file_size count,
				                   Vfs::file_size &out_count) override
				{
					Genode::log("Event_vfs_handle::write()");

					return WRITE_ERR_IO;
				}

				bool read_ready() { return true; }
		};

	public:

		Event_file_system(Genode::Env &env,
		                  Genode::Allocator &alloc,
		                  Genode::Xml_node config)
		:
			Single_file_system(Vfs::Node_type::CONTINUOUS_FILE, name(),
			                   Vfs::Node_rwx::ro(), config),
			_event(env)
		{ }

		~Event_file_system()
		{
		}

		static char const *name()   { return "event"; }
		char const *type() override { return "event"; }

		/*********************************
		 ** Directory service interface **
		 *********************************/

		Open_result open(char const         *path, unsigned,
		                 Vfs::Vfs_handle   **out_handle,
		                 Genode::Allocator  &alloc) override
		{
			if (!_single_file(path))
				return OPEN_ERR_UNACCESSIBLE;

			*out_handle = new (alloc)
				Event_vfs_handle(*this, *this, alloc, _event);
			return OPEN_OK;
		}

};

#endif /* _EVENT_FILE_SYSTEM_H_ */
