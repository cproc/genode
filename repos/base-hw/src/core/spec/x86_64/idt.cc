#include "idt.h"

extern int _mt_begin;
extern int _mt_idt;

using namespace Genode;

class Descriptor
{
	private:
		uint16_t _limit;
		uint64_t _base;

	public:
		Descriptor(uint16_t l, uint64_t b) : _limit(l), _base (b) {};
} __attribute__((packed));


addr_t Idt::_virt_idt_addr(addr_t const virt_base)
{
	addr_t const phys      = (addr_t)&_mt_idt;
	addr_t const phys_base = (addr_t)&_mt_begin;
	return virt_base + (phys - phys_base);
}

void Idt::setup()
{
	/* TODO: Calculate from _mt_isrs label */
	uint64_t base = 0;

	for (unsigned vec = 0; vec < SIZE_IDT; vec++) {
		/* ISRs are padded to 4 bytes */
		base = vec * 0xc;

		_table[vec].offset_15_00 = base & 0xffff;
		_table[vec].segment_sel  = 8;
		_table[vec].flags        = 0x8e00;
		_table[vec].offset_31_16 = (base >> 16) & 0xffff;
		_table[vec].offset_63_32 = (base >> 32) & 0xffff;
	}

	/* Set DPL of syscall entry to 3 */
	_table[SYSCALL_VEC].flags = _table[SYSCALL_VEC].flags | 0x6000;
}


void Idt::load(addr_t const virt_base)
{
	asm volatile ("lidt %0" : : "m" (Descriptor (sizeof(_table) - 1,
				  _virt_idt_addr(virt_base))));
}
