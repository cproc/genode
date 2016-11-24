/*
 * \brief  Allocator interface
 * \author Norman Feske
 * \date   2010-09-27
 */

#ifndef _NANO3D__ALLOCATOR_H_
#define _NANO3D__ALLOCATOR_H_

namespace Nano3d {

	class Allocator
	{
		public:

			virtual void *alloc(unsigned long size) = 0;
			virtual void  free(void *ptr, unsigned long size) = 0;
	};
}

#endif /* _NANO3D__ALLOCATOR_H_ */
