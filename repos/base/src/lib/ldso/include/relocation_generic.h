/**
 * \brief  Generic relocation classes
 * \author Sebastian Sumpf
 * \date   2014-10-26
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__RELOCATION_GENERIC_H_
#define _INCLUDE__RELOCATION_GENERIC_H_

#include <linker.h>

namespace Linker
{
	struct Plt_got;
	template <typename REL, unsigned TYPE, bool DIV> class Reloc_jmpslot_generic;
	template <typename REL, unsigned TYPE, unsigned JMPSLOT> struct Reloc_plt_generic;
	template <typename REL, unsigned TYPE> struct Reloc_bind_now_generic;
}


/**
 * Set 2nd and 3rd GOT entry (see: SYSTEM V APPLICATION BINARY INTERFACE
 * Intel386 Architecture Processor Supplement - 5.9
 */
struct Linker::Plt_got
{
	Plt_got(Dag const *dag, Elf::Addr *pltgot)
	{
		if (verbose_relocation)
			PDBG("OBJ: %s (%p)", dag->obj->name(), dag);

		pltgot[1] = (Elf::Addr) dag;        /* ELF object */
		pltgot[2] = (Elf::Addr) &_jmp_slot; /* Linker entry */
	}
};


/**
 * PLT relocations
 */
template<typename REL, unsigned TYPE, unsigned JMPSLOT>
struct Linker::Reloc_plt_generic
{
	Reloc_plt_generic(Object const *obj, D_tag const type,
	                  Elf::Rel const *start, unsigned long size)
	{
		if (type != TYPE) {
			PERR("LD: Unsupported PLT relocation type: %u", type);
			throw Incompatible();
		}

		REL const *rel = (REL const *)start;
		REL const *end = rel + (size / sizeof(REL));
		for (; rel < end; rel++) {

			if (rel->type() != JMPSLOT) {
				PERR("LD: Unsupported PLT relocation %u", rel->type());
				throw Incompatible();
			}

			/* find relocation address and add relocation base */
			Elf::Addr *addr = (Elf::Addr *)(obj->reloc_base() + rel->offset);
			*addr          += obj->reloc_base();
		}
	}
};


/**
 * Generic jmp slot handling
 */
template <typename REL, unsigned TYPE, bool DIV>
class Linker::Reloc_jmpslot_generic
{
		Elf::Addr *_addr = 0;

	public:

		Reloc_jmpslot_generic(Dag const *dag, unsigned const type, Elf::Rel const* pltrel,
		                      Elf::Size const index)
		{
			if (type != TYPE) {
				PERR("LD: Unsupported JMP relocation type: %u", type);
				throw Incompatible();
			}
		
			REL const *rel = &((REL *)pltrel)[index / (DIV ? sizeof(REL) : 1)];
			Elf::Sym const *sym;
			Elf::Addr      reloc_base;

			if (!(sym = locate_symbol(rel->sym(), dag, &reloc_base))) {
				PWRN("LD: Symbol not found");
				return;
			}

			/* write address of symbol to jump slot */
			_addr  = (Elf::Addr *)(dag->obj->reloc_base() + rel->offset);
			*_addr = reloc_base + sym->st_value;


			if (verbose_relocation) {
				PDBG("jmp: rbase " EFMT " s: %p sval: " EFMT, reloc_base, sym, sym->st_value);
				PDBG("jmp_slot at %p -> " EFMT, _addr, *_addr);
			}
		}

		Elf::Addr target_addr() const { return *_addr; }
};


/**
 * Relocate jump slots immediately
 */
template <typename REL, unsigned TYPE>
struct Linker::Reloc_bind_now_generic
{
	Reloc_bind_now_generic(Dag const *dag, Elf::Rel const *pltrel, unsigned long const size)
	{
		Elf::Size last_index = size / sizeof(REL);

		for (Elf::Size index = 0; index < last_index; index++)
			Reloc_jmpslot_generic<REL, TYPE, false> reloc(dag, TYPE, pltrel, index);
	}
};

#endif /* _INCLUDE__RELOCATION_GENERIC_H_ */
