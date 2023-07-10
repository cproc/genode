/*
 * \brief  Architecture-specific GDB protocol support
 * \author Norman Feske
 * \date   2023-05-15
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
//#include <cpu/cpu_state.h>
#include <util/endian.h>

/* monitor includes */
#include <gdb_arch.h>
#include <monitored_thread.h>

using namespace Monitor;


void Monitor::Gdb::print_registers(Output &out, Cpu_state const &cpu)
{
	uint64_t const values_64bit[] = {
		cpu.rax, cpu.rbx, cpu.rcx, cpu.rdx, cpu.rsi, cpu.rdi, cpu.rbp, cpu.sp,
		cpu.r8,  cpu.r9,  cpu.r10, cpu.r11, cpu.r12, cpu.r13, cpu.r14, cpu.r15,
		cpu.ip };

	for (uint64_t value : values_64bit)
		print(out, Gdb_hex(host_to_big_endian(value)));

	uint32_t const values_32bit[] = {
		uint32_t(cpu.eflags), uint32_t(cpu.cs), uint32_t(cpu.ss),
		0 /* ds */, 0 /* es */, 0 /* fs */, 0 /* gs */ };

	for (uint32_t value : values_32bit)
		print(out, Gdb_hex(host_to_big_endian(value)));
}


void Monitor::Gdb::scan_registers(Const_byte_range_ptr const &in, Cpu_state &cpu)
{
	addr_t * const values_64bit[] = {
		&cpu.rax, &cpu.rbx, &cpu.rcx, &cpu.rdx, &cpu.rsi, &cpu.rdi, &cpu.rbp, &cpu.sp,
		&cpu.r8,  &cpu.r9,  &cpu.r10, &cpu.r11, &cpu.r12, &cpu.r13, &cpu.r14, &cpu.r15,
		&cpu.ip };

	for (size_t i = 0; i < sizeof(values_64bit) / sizeof(addr_t); i++) {
		Genode::log("scan_registers(): i: ", i);
		with_skipped_bytes(in, i * sizeof(addr_t) * 2,
		                   [&] (Const_byte_range_ptr const &in) {
			with_max_bytes(in, sizeof(addr_t) * 2, [&] (Const_byte_range_ptr const &in) {
				char null_terminated[sizeof(addr_t) * 2 + 1] { };
				memcpy(null_terminated, in.start,
				       min(sizeof(null_terminated) - 1, in.num_bytes));
				Genode::log("scan_registers(): ", Genode::Cstring(null_terminated));
				addr_t value = 0;
				ascii_to_unsigned(null_terminated, value, 16);
				Genode::log("scan_registers(): ", Genode::Hex(value));
				*values_64bit[i] = big_endian_to_host(value);
				Genode::log("scan_registers(): ", Genode::Hex(*values_64bit[i]));
			});
		});
	}

	addr_t * const values_32bit[] = {
		&cpu.eflags, &cpu.cs, &cpu.ss };

	for (size_t i = 0; i < sizeof(values_32bit) / sizeof(addr_t); i++) {
		Genode::log("scan_registers(): i: ", i);
		with_skipped_bytes(in, (sizeof(values_64bit) * 2) + (i * sizeof(uint32_t) * 2),
		                   [&] (Const_byte_range_ptr const &in) {
			with_max_bytes(in, sizeof(uint32_t) * 2, [&] (Const_byte_range_ptr const &in) {
				char null_terminated[sizeof(uint32_t) * 2 + 1] { };
				memcpy(null_terminated, in.start,
				       min(sizeof(null_terminated) - 1, in.num_bytes));
				Genode::log("scan_registers(): ", Genode::Cstring(null_terminated));
				uint32_t value = 0;
				ascii_to_unsigned(null_terminated, value, 16);
				Genode::log("scan_registers(): ", Genode::Hex(value));
				*values_32bit[i] = big_endian_to_host(value);
				Genode::log("scan_registers(): ", Genode::Hex(*values_32bit[i]));
			});
		});
	}
}


void Monitor::Monitored_thread::_handle_exception()
{
	stop_state = Stop_state::STOPPED_REPLY_PENDING;

	Thread_state thread_state = _real.call<Rpc_get_state>();
Genode::warning("_handle_exception(): trapno: ", Genode::Hex(thread_state.trapno),
        ", ip: ", Genode::Hex(thread_state.ip));

	if (thread_state.trapno == Cpu_state::Cpu_exception::BREAKPOINT) {
		thread_state.ip -= Gdb::breakpoint_len;
		_real.call<Rpc_set_state>(thread_state);
	}

	if (_wait) {
Genode::warning("_handle_exception(): first instruction address: ", Genode::Hex(_first_instruction_addr));
		_wait = false;
		_thread_monitor.remove_initial_breakpoint(_pd, _first_instruction_addr,
			                                      _original_first_instruction);
		stop_reply_signal = Stop_reply_signal::STOP;
	} else {
		switch(thread_state.trapno) {
		case Cpu_state::Cpu_exception::DIVIDE_ERROR:
			stop_reply_signal = Stop_reply_signal::FPE;
			break;
		case Cpu_state::Cpu_exception::DEBUG:
			stop_reply_signal = Stop_reply_signal::TRAP;
			break;
		case Cpu_state::Cpu_exception::BREAKPOINT:
			stop_reply_signal = Stop_reply_signal::TRAP;
			break;
		case Cpu_state::Cpu_exception::UNDEFINED_INSTRUCTION:
			stop_reply_signal = Stop_reply_signal::ILL;
			break;
		case Cpu_state::Cpu_exception::GENERAL_PROTECTION:
			stop_reply_signal = Stop_reply_signal::SEGV;
			break;
		default:
			stop_reply_signal = Stop_reply_signal::TRAP;
		}
	}

	_thread_monitor.thread_stopped(_pd, *this);

Genode::log("_handle_exception() finished");
}
