/*
 * \brief  Symlink file-system node
 * \author Norman Feske
 * \date   2012-04-11
 */

#ifndef _SYMLINK_H_
#define _SYMLINK_H_

/* local includes */
#include <node.h>

namespace File_system {

	class Symlink : public Node
	{
		private:

			char _link_to[MAX_PATH_LEN];

		public:

			Symlink(char const *name) { Node::name(name); }

			size_t read(char *dst, size_t len, seek_off_t seek_offset)
			{
				Genode::strncpy(dst, _link_to, min(len + 1, sizeof(_link_to)));
				PDBG("dst = %s, len = %zu", dst, len);
				return 0;
			}

			size_t write(char const *src, size_t len, seek_off_t seek_offset)
			{
				Genode::strncpy(_link_to, src, min(len + 1, sizeof(_link_to)));
				PDBG("link_to = %s, len = %zu", _link_to, len);
				return 0;
			}

			file_size_t length() const { return strlen(_link_to) + 1; }
	};
}

#endif /* _SYMLINK_H_ */
