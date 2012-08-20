/*
 * \brief  TAR file-system node
 * \author Christian Prochaska
 * \date   2012-08-20
 */

#ifndef _NODE_H_
#define _NODE_H_

/* Genode includes */
#include <file_system_session/file_system_session.h>
#include <os/path.h>


namespace File_system {

	class Record
	{
		private:

			char _name[100];
			char _mode[8];
			char _uid[8];
			char _gid[8];
			char _size[12];
			char _mtime[12];
			char _checksum[8];
			char _type[1];
			char _linked_name[100];

			/**
			 * Convert ASCII-encoded octal number to unsigned value
			 */
			template <typename T>
			unsigned long _read(T const &field) const
			{
				/*
				 * Copy-out ASCII string to temporary buffer that is
				 * large enough to host an additional zero.
				 */
				char buf[sizeof(field) + 1];
				strncpy(buf, field, sizeof(buf));

				unsigned long value = 0;
				ascii_to(buf, &value, 8);
				return value;
			}

		public:

			/* length of on data block in tar */
			enum { BLOCK_LEN = 512 };

			/* record type values */
			enum { TYPE_FILE    = 0, TYPE_HARDLINK = 1,
				   TYPE_SYMLINK = 2, TYPE_DIR      = 5 };

			size_t             size() const { return _read(_size); }
			unsigned            uid() const { return _read(_uid);  }
			unsigned            gid() const { return _read(_gid);  }
			unsigned           mode() const { return _read(_mode); }
			unsigned           type() const { return _read(_type); }
			char const        *name() const { return _name;        }
			char const *linked_name() const { return _linked_name; }

			void *data() const { return (char *)this + BLOCK_LEN; }
	};


	class Node
	{
		protected:

			Record *_record;

		public:

			Node(Record *record) : _record(record) { }

			Record const *record() const { return _record; }

			/*
			 * A generic Node object can be created to represent a file or
			 * directory by its name without opening it, so the functions
			 * of this class must not be abstract.
			 */

			virtual size_t read(char *dst, size_t len, seek_off_t)
			{
				PERR("read() called on generic Node object");
				return 0;
			}

			virtual size_t write(char const *src, size_t len, seek_off_t)
			{
				PERR("write() called on generic Node object");
				return 0;
			}
	};

}

#endif /* _NODE_H_ */
