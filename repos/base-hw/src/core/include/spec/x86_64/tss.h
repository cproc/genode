#ifndef _TSS_H_
#define _TSS_H_

#include <base/stdint.h>

namespace Genode
{
	/**
	 * Task State Segment (TSS)
	 *
	 * See Intel SDM Vol. 3A, section 7.7
	 */
	class Tss;
}

class Genode::Tss
{
	private:

		enum {
			TSS_SELECTOR = 0x28,
		};

		uint32_t : 32;
		addr_t rsp0;
		addr_t rsp1;
		addr_t rsp2;
		uint64_t : 64;
		addr_t ist[7];
		uint64_t : 64;
		uint16_t : 16;
		uint16_t iomap_base;

	public:

		/**
		 * Setup TSS.
		 *
		 * \param virt_base  virtual base address of mode transition pages
		 */
		void setup(addr_t const virt_base);

		/**
		 * Load TSS into TR.
		 */
		void load();

}__attribute__((packed));

#endif /* _TSS_H_ */
