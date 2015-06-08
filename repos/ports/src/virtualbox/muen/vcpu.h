
#include <base/stdint.h>

struct Cpu_registers
{
	Genode::uint64_t Cr2;
	Genode::uint64_t Rax;
	Genode::uint64_t Rbx;
	Genode::uint64_t Rcx;
	Genode::uint64_t Rdx;
	Genode::uint64_t Rdi;
	Genode::uint64_t Rsi;
	Genode::uint64_t Rbp;
	Genode::uint64_t R08;
	Genode::uint64_t R09;
	Genode::uint64_t R10;
	Genode::uint64_t R11;
	Genode::uint64_t R12;
	Genode::uint64_t R13;
	Genode::uint64_t R14;
	Genode::uint64_t R15;
} __attribute__((packed));

struct Subject_state
{
	struct Cpu_registers Regs;
	Genode::uint64_t Exit_reason;
	Genode::uint64_t Exit_qualification;
	Genode::uint64_t Guest_phys_addr;
	Genode::uint64_t Interrupt_info;
	Genode::uint64_t Instruction_len;
	Genode::uint64_t Rip;
	Genode::uint64_t Cs;
	Genode::uint64_t Rsp;
	Genode::uint64_t Ss;
	Genode::uint64_t Cr0;
	Genode::uint64_t Shadow_cr0;
	Genode::uint64_t Cr3;
	Genode::uint64_t Cr4;
	Genode::uint64_t Rflags;
	Genode::uint64_t Ia32_efer;
} __attribute__((packed));

