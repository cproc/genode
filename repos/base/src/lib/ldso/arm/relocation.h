/**
 * \brief  ARM specific relocations
 * \author Sebastian Sumpf
 * \date   2014-10-26
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__ARM__RELOCATION_H_
#define _INCLUDE__ARM__RELOCATION_H_

#include <relocation_generic.h>

namespace Linker {
	enum Reloc_types {
		R_ABS32    = 2,
		R_REL32    = 3,
		R_COPY     = 20,
		R_GLOB_DAT = 21,
		R_JMPSLOT  = 22,
		R_RELATIVE = 23,
	};

	class Reloc_non_plt;

	typedef Reloc_plt_generic<Elf::Rel, DT_REL, R_JMPSLOT> Reloc_plt;
	typedef Reloc_jmpslot_generic<Elf::Rel, DT_REL, false> Reloc_jmpslot;
	typedef Reloc_bind_now_generic<Elf::Rel, DT_REL>       Reloc_bind_now;
};


class Linker::Reloc_non_plt
{
	private:

		Dag const *_dag;

		void _rel32(Elf::Rel const *rel, Elf::Addr *addr)
		{
			Elf::Addr reloc_base;
			Elf::Sym  const *sym;

			if (!(sym = locate_symbol(rel->sym(), _dag, &reloc_base)))
				return;

			/* S + A - P */
			*addr = reloc_base + sym->st_value - (Elf::Addr)addr + *addr;
			trace("REL32", (unsigned long)addr, *addr, 0);
		}

		/**
		 * Copy relocations, these are just for the main program, we can do them
		 * safely here since all other DSO are loaded, relocated, and constructed at
		 * this point
		 */
		void _copy(Elf::Rel const *rel, Elf::Addr *addr)
		{
			if (!_dag->obj->is_binary()) {
				PERR("LD: Copy relocation in DSO (%s at %p)", _dag->obj->name(), addr);
				throw Incompatible();
			}

			Elf::Sym const *sym;
			Elf::Addr      reloc_base;

			 /* search symbol in other objects, do not return undefined symbols */
			if (!(sym = locate_symbol(rel->sym(), _dag, &reloc_base, false, true))) {
				PWRN("LD: Symbol not found");
				return;
			}

			Elf::Addr src = reloc_base + sym->st_value;
			Genode::memcpy(addr, (void *)src, sym->st_size);
			
			if (verbose_relocation)
				PDBG("Copy relocation: " EFMT " -> %p (0x%zx bytes)", src, addr,sym->st_size);
		}

		void _glob_dat(Elf::Rel const *rel, Elf::Addr *addr, bool no_addend)
		{
			Elf::Addr reloc_base;
			Elf::Sym  const *sym;

			if (!(sym = locate_symbol(rel->sym(), _dag, &reloc_base)))
				return;

			Elf::Addr addend = no_addend ? 0 : *addr;

			/* S + A */
			*addr = addend + reloc_base + sym->st_value;
			trace("GLOB_DAT", (unsigned long)addr, *addr, 0);
		}

		void _relative(Elf::Addr *addr)
		{
			/*
			 * This ommits the linker and the binary, the linker has relative
			 * relocations within its text-segment (e.g., 'initial_sp' and friends), which
			 * we cannot write to from here).
			 */
			if (_dag->obj->reloc_base())
				*addr += _dag->obj->reloc_base();
		}

	public:

		Reloc_non_plt(Dag const *, Elf::Rela const *, unsigned long)
		{
			PERR("LD: DT_RELA not supported");
			throw Incompatible();
		}

		Reloc_non_plt(Dag const *dag, Elf::Rel const *rel, unsigned long size, bool second_pass)
		: _dag(dag)
		{
			Elf::Rel const *end = rel + (size / sizeof(Elf::Rel));
			for (; rel < end; rel++) {
				Elf::Addr *addr = (Elf::Addr *)(_dag->obj->reloc_base() + rel->offset);

				if (second_pass && rel->type() != R_GLOB_DAT)
					continue;

				switch (rel->type()) {

					case R_REL32   : _rel32(rel, addr);                 break;
					case R_COPY    : _copy(rel, addr);                  break;
					case R_ABS32   :
					case R_GLOB_DAT: _glob_dat(rel, addr, second_pass); break;
					case R_RELATIVE: _relative(addr);                   break;
					default:
						trace("UNKREL", rel->type(), 0, 0);
						if (_dag->root) {
							PWRN("LD: Unkown relocation %u", rel->type());
							throw Incompatible();
						}
						break;
				}
			}
		}
};

#endif /* _INCLUDE__ARM__RELOCATION_H_ */
