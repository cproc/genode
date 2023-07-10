/*
 * \brief  GDB stub
 * \author Norman Feske
 * \date   2023-05-11
 */

/*
 * Copyright (C) 2023 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _GDB_STUB_H_
#define _GDB_STUB_H_

#include <inferior_cpu.h>
#include <memory_accessor.h>
#include <gdb_command.h>
#include <gdb_response.h>
#include <gdb_arch.h>

namespace Monitor { namespace Gdb { struct State; } }


struct Monitor::Gdb::State : Noncopyable
{
	Inferiors &inferiors;

	struct Thread_list
	{
		char   _buf[1024*16] { };
		size_t _len = 0;

		Thread_list(Inferiors const &inferiors)
		{
			Xml_generator xml(_buf, sizeof(_buf), "threads", [&] {
				inferiors.for_each<Inferior_pd const &>([&] (Inferior_pd const &inferior) {
					inferior.for_each_thread([&] (Monitored_thread const &thread) {
						xml.node("thread", [&] {
							String<32> const id("p", inferior.id(), ".", thread.id());
							xml.attribute("id",   id);
							xml.attribute("core", 0);
							xml.attribute("name", thread._name); }); }); }); });

			_len = strlen(_buf);
		}

		void with_bytes(auto const &fn) const
		{
			Const_byte_range_ptr const ptr { _buf, _len };
			fn(ptr);
		}
	};

	Memory_accessor &_memory_accessor;

	struct Current : Noncopyable
	{
		Inferior_pd &pd;

		struct Thread
		{
			Monitored_thread &thread;

			Thread(Monitored_thread &thread) : thread(thread) { }
		};

		Constructible<Thread> thread { };

		Current(Inferior_pd &pd) : pd(pd) { }
	};

	Constructible<Current> _current { };

	bool non_stop_mode { false };

	/**
	 * Only one stop notification is sent directly, then
	 * additional stop replies are sent as response to 'vStopped'.
	 */
	bool notification_in_progress { false };

	void flush(Inferior_pd &pd)
	{
		if (_current.constructed() && _current->pd.id() == pd.id())
			_current.destruct();
	}

	void flush(Monitored_thread &thread)
	{
		if (_current.constructed() &&
		    _current->thread.constructed() &&
		    (&_current->thread->thread == &thread))
			_current->thread.destruct();
	}

	size_t read_memory(Inferior_pd &pd, Memory_accessor::Virt_addr at, Byte_range_ptr const &dst)
	{
		return _memory_accessor.read(pd, at, dst);
	}

	size_t write_memory(Inferior_pd &pd, Memory_accessor::Virt_addr at, Const_byte_range_ptr const &src)
	{
		return _memory_accessor.write(pd, at, src);
	}

	size_t read_memory(Memory_accessor::Virt_addr at, Byte_range_ptr const &dst)
	{
		if (_current.constructed())
			return _memory_accessor.read(_current->pd, at, dst);

		warning("attempt to read memory without a current target");
		return 0;
	}

	size_t write_memory(Memory_accessor::Virt_addr at, Const_byte_range_ptr const &src)
	{
		if (_current.constructed())
			return _memory_accessor.write(_current->pd, at, src);

		warning("attempt to write memory without a current target");
		return 0;
	}

	bool current_defined() const { return _current.constructed(); }

	/**
	 * Select current inferior and thread (id == 0 means any).
	 *
	 * GDB initially sends a Hgp0.0 command but assumes that inferior 1
	 * is current. Avoid losing the default current inferior as set by
	 * 'Main::_create_session' by keeping the previously chosen inferior.
	 */
	void current(Inferiors::Id pid, Threads::Id tid)
	{
		if ((pid.value == 0) && _current.constructed()) {

			pid.value = _current->pd.id();

			if ((tid.value == 0) && _current->thread.constructed()) {
				/* keep the current thread */
				return;
			}
		}

		_current.destruct();

		inferiors.for_each<Inferior_pd &>([&] (Inferior_pd &inferior) {

			if ((_current.constructed() &&
			     _current->thread.constructed()) ||
			    ((pid.value > 0) && (inferior.id() != pid.value)))
				return;

			_current.construct(inferior);

			inferior._threads.for_each<Monitored_thread &>([&] (Monitored_thread &thread) {

				if (_current->thread.constructed() ||
				    ((tid.value > 0) && (thread.id() != tid.value)))
				    return;

				_current->thread.construct(thread);
			});
		});
	}

	void with_current_thread_state(auto const &fn)
	{
		Thread_state thread_state { };

		if (_current.constructed() && _current->thread.constructed()) {
			try {
				thread_state = _current->thread->thread._real.call<Cpu_thread::Rpc_get_state>();
			} catch (Cpu_thread::State_access_failed) {
				warning("unable to access state of thread ", _current->thread->thread.id());
			}
		}

		fn(thread_state);
	};

	State(Inferiors &inferiors, Memory_accessor &memory_accessor)
	:
		inferiors(inferiors), _memory_accessor(memory_accessor)
	{ }
};


/*
 * The command types within the 'Gdb::Cmd' namespace deliberately do not follow
 * Genode's coding style regarding the use of upper/lower case letters.
 *
 * The types are named precisely atfer the corresponding commands of the GDB
 * prototol, which are case sensitive.
 */

namespace Monitor { namespace Gdb { namespace Cmd {


/**
 * Protocol negotiation
 */
struct qSupported : Command_with_separator
{
	qSupported(Commands &c) : Command_with_separator(c, "qSupported") { }

	void execute(State &, Const_byte_range_ptr const &args, Output &out) const override
	{
		/* check for expected GDB features */
		bool gdb_supports_multiprocess = false,
		     gdb_supports_vcont        = false;

		for_each_argument(args, Sep {';'}, [&] (Const_byte_range_ptr const &arg) {
			if (equal(arg, "multiprocess+"))   gdb_supports_multiprocess = true;
			if (equal(arg, "vContSupported+")) gdb_supports_vcont        = true;
		});

		if (!gdb_supports_multiprocess) warning("GDB lacks multi-process support");
		if (!gdb_supports_vcont)        warning("GDB lacks vcont support");

		/* tell GDB about our features */
		gdb_response(out, [&] (Output &out) {
			print(out, "PacketSize=", Gdb_hex(GDB_PACKET_MAX_SIZE), ";");
			print(out, "vContSupported+;");
			print(out, "qXfer:features:read+;");  /* XML target descriptions */
			print(out, "qXfer:threads:read+;");
			print(out, "multiprocess+;");
			print(out, "QNonStop+;");
		});
	}
};


extern "C" char _binary_gdb_target_xml_start[];
extern "C" char _binary_gdb_target_xml_end[];


/**
 * Query XML-based information
 */
struct qXfer : Command_with_separator
{
	qXfer(Commands &c) : Command_with_separator(c, "qXfer") { }

	struct Raw_data_ptr : Const_byte_range_ptr
	{
		Raw_data_ptr(char const *start, char const *end)
		:
			Const_byte_range_ptr(start, end - start)
		{ }
	};

	struct Window
	{
		size_t offset, len;

		static Window from_args(Const_byte_range_ptr const &args)
		{
			return { .offset = comma_separated_hex_value(args, 0, 0UL),
			         .len    = comma_separated_hex_value(args, 1, 0UL) };
		}
	};

	static void _send_window(Output &out, Const_byte_range_ptr const &total_bytes, Window const window)
	{
		with_skipped_bytes(total_bytes, window.offset, [&] (Const_byte_range_ptr const &bytes) {
			with_max_bytes(bytes, window.len, [&] (Const_byte_range_ptr const &bytes) {
				gdb_response(out, [&] (Output &out) {
					char const *prefix = (window.offset + window.len < total_bytes.num_bytes)
					                   ? "m" : "l"; /* 'last' marker */
					print(out, prefix, Cstring(bytes.start, bytes.num_bytes)); }); }); });
	}

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		bool handled = false;

		with_skipped_prefix(args, "features:read:target.xml:", [&] (Const_byte_range_ptr const &args) {
			Raw_data_ptr const total_bytes { _binary_gdb_target_xml_start, _binary_gdb_target_xml_end };
			_send_window(out, total_bytes, Window::from_args(args));
			handled = true;
		});

		with_skipped_prefix(args, "threads:read::", [&] (Const_byte_range_ptr const &args) {
			State::Thread_list const thread_list(state.inferiors);
			thread_list.with_bytes([&] (Const_byte_range_ptr const &bytes) {
				_send_window(out, bytes, Window::from_args(args)); });
			handled = true;
		});

		if (!handled)
			warning("GDB ", name, " command unsupported: ", Cstring(args.start, args.num_bytes));
	}
};


struct vMustReplyEmpty : Command_without_separator
{
	vMustReplyEmpty(Commands &c) : Command_without_separator(c, "vMustReplyEmpty") { }

	void execute(State &, Const_byte_range_ptr const &, Output &out) const override
	{
		gdb_response(out, [&] (Output &) { });
	}
};


/**
 * Set current thread
 */
struct H : Command_without_separator
{
	H(Commands &commands) : Command_without_separator(commands, "H") { }

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("H command args: ", Cstring(args.start, args.num_bytes));

		/* 'g' for other operations */
		with_skipped_prefix(args, "g", [&] (Const_byte_range_ptr const &args) {

			int pid = 0, tid = 0;
			thread_id(args, pid, tid);

			if (pid == -1) {
				Genode::error("pid -1 not supported");
				gdb_error(out, 1);
				return;
			}

			if (tid == -1) {
				Genode::error("tid -1 not supported");
				gdb_error(out, 1);
				return;
			}

			state.current(Inferiors::Id { (unsigned)pid },
			              Threads::Id { (unsigned)tid });

			gdb_ok(out);
		});

		with_skipped_prefix(args, "c-", [&] (Const_byte_range_ptr const &) {
			gdb_error(out, 1); });
	}
};


/**
 * Enable/disable non-stop mode
 */
struct QNonStop : Command_with_separator
{
	QNonStop(Commands &commands) : Command_with_separator(commands, "QNonStop") { }

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("QNonStop command args: ", Cstring(args.start, args.num_bytes));

		with_null_terminated(args, [&] (char const * const str) {
			unsigned non_stop_mode { 0 };
			ascii_to(str, non_stop_mode);
			state.non_stop_mode = non_stop_mode;
		});

		gdb_ok(out);
	}
};


/**
 * Symbol-lookup prototol (not used)
 */
struct qSymbol : Command_with_separator
{
	qSymbol(Commands &commands) : Command_with_separator(commands, "qSymbol") { }

	void execute(State &, Const_byte_range_ptr const &, Output &out) const override
	{
		gdb_ok(out);
	}
};


/**
 * Query trace status
 */
struct qTStatus : Command_without_separator
{
	qTStatus(Commands &commands) : Command_without_separator(commands, "qTStatus") { }

	void execute(State &, Const_byte_range_ptr const &, Output &out) const override
	{
		gdb_response(out, [&] (Output &) { });
	}
};


/**
 * Query current thread ID
 */
struct qC : Command_without_separator
{
	qC(Commands &commands) : Command_without_separator(commands, "qC") { }

	void execute(State &, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("qC: ", Cstring(args.start, args.num_bytes));

		gdb_response(out, [&] (Output &) { });
	}
};


/**
 * Query attached state
 */
struct qAttached : Command_without_separator
{
	qAttached(Commands &commands) : Command_without_separator(commands, "qAttached") { }

	void execute(State &, Const_byte_range_ptr const &, Output &out) const override
	{
		gdb_response(out, [&] (Output &out) {
			print(out, "1"); });
	}
};


/**
 * Query text/data segment offsets
 */
struct qOffsets : Command_without_separator
{
	qOffsets(Commands &commands) : Command_without_separator(commands, "qOffsets") { }

	void execute(State &, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("qOffsets: ", Cstring(args.start, args.num_bytes));

		gdb_response(out, [&] (Output &) { });
	}
};


/**
 * Query halt reason
 */
struct ask : Command_without_separator
{
	ask(Commands &c) : Command_without_separator(c, "?") { }

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("? command args: ", Cstring(args.start, args.num_bytes));

		bool handled = false;

//		if (state.non_stop_mode) {

		/* check current thread first */
		if (state._current.constructed() &&
			state._current->thread.constructed() &&
			state._current->thread->thread.stopped_status_pending) {
			Genode::log("?: current thread is stopped");
			Inferior_pd &inferior = state._current->pd;
			Monitored_thread &thread = state._current->thread->thread;
			thread.stopped_status_pending = false;
			long unsigned int pid = inferior.id();
			long unsigned int tid = thread.id();
			Genode::log("?: current thread: ", pid, ".", tid);
			gdb_response(out, [&] (Output &out) {
				print(out, "T", Gdb_hex((uint8_t)thread.stop_reason),
				           "thread:p", Gdb_hex(pid), ".", Gdb_hex(tid), ";");
			});
			return;
		}

		state.inferiors.for_each<Inferior_pd const &>([&] (Inferior_pd const &inferior) {
			inferior.for_each_thread([&] (Monitored_thread &thread) {

				if (handled)
					return;

				if (!thread.stopped_status_pending)
					return;

				thread.stopped_status_pending = false;
				long unsigned int pid = inferior.id();
				long unsigned int tid = thread.id();
				Genode::log("?: found stopped thread: ", pid, ".", tid);
				gdb_response(out, [&] (Output &out) {
					print(out, "T", Gdb_hex((uint8_t)thread.stop_reason),
					           "thread:p", Gdb_hex(pid), ".", Gdb_hex(tid), ";");
				});

				handled = true;
			});
		});

		if (!handled)
			gdb_ok(out);
	}
};


/**
 * Read registers
 */
struct g : Command_without_separator
{
	g(Commands &c) : Command_without_separator(c, "g") { }

	void execute(State &state, Const_byte_range_ptr const &, Output &out) const override
	{
		log("-> execute g");

		gdb_response(out, [&] (Output &out) {
			state.with_current_thread_state([&] (Thread_state const &thread_state) {
				print_registers(out, thread_state); }); });
	}
};


/**
 * Read memory
 */
struct m : Command_without_separator
{
	m(Commands &c) : Command_without_separator(c, "m") { }

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		addr_t const addr = comma_separated_hex_value(args, 0, addr_t(0));
		size_t const len  = comma_separated_hex_value(args, 1, 0UL);

		gdb_response(out, [&] (Output &out) {

			for (size_t pos = 0; pos < len; ) {

				char chunk[16*1024] { };

				size_t const remain_len = len - pos;
				size_t const num_bytes  = min(sizeof(chunk), remain_len);

				size_t const read_len =
					state.read_memory(Memory_accessor::Virt_addr { addr + pos },
					                  Byte_range_ptr(chunk, num_bytes));

				for (unsigned i = 0; i < read_len; i++)
					print(out, Gdb_hex(chunk[i]));

				pos += read_len;

				if (read_len < num_bytes)
					break;
			}
		});
	}
};


/**
 * Write memory (binary data)
 */
struct X : Command_without_separator
{
	X(Commands &c) : Command_without_separator(c, "X") { }

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		addr_t const addr = comma_separated_hex_value(args, 0, addr_t(0));
		size_t const len  = comma_separated_hex_value(args, 1, 0UL);

Genode::log("X: addr: ", Genode::Hex(addr), ", len: ", len);

		if (len == 0) {
			/* packet support probing */
			gdb_ok(out);
			return;
		}

		size_t written_num_bytes { 0 };

		with_argument(args, Sep {':'}, 1, [&] (Const_byte_range_ptr const &arg) {
			Genode::log("X: start: ", (void*)arg.start);
			Genode::log("X: num_bytes: ", arg.num_bytes);
			if (arg.num_bytes != len)
				return;

			written_num_bytes =
				state.write_memory(Memory_accessor::Virt_addr { addr }, arg);
		});

		if (written_num_bytes == len)
			gdb_ok(out);
		else
			gdb_error(out, 1);
	}
};


/**
 * Query liveliness of thread ID
 */
struct T : Command_without_separator
{
	T(Commands &c) : Command_without_separator(c, "T") { }

	void execute(State &, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("T command args: ", Cstring(args.start, args.num_bytes));

		gdb_ok(out);
	}
};


/**
 * Disconnect
 */
struct D : Command_with_separator
{
	D(Commands &c) : Command_with_separator(c, "D") { }

	void execute(State &, Const_byte_range_ptr const &, Output &out) const override
	{
		/* TODO: resume all threads */
		gdb_ok(out);
	}
};


/**
 * Enable extended mode
 */
struct bang : Command_without_separator
{
	bang(Commands &c) : Command_without_separator(c, "!") { }

	void execute(State &, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("! command args: ", Cstring(args.start, args.num_bytes));

		gdb_ok(out);
	}
};


/**
 * Report stopped threads in non-stop mode
 */
struct vStopped : Command_without_separator
{
	vStopped(Commands &c) : Command_without_separator(c, "vStopped") { }

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("vStopped command args: ", Cstring(args.start, args.num_bytes));

		bool handled = false;

		/* check current thread first */
		if (state._current.constructed() &&
			state._current->thread.constructed() &&
			state._current->thread->thread.stopped_status_pending) {
			Genode::log("vStopped: current thread is stopped");
			Inferior_pd &inferior = state._current->pd;
			Monitored_thread &thread = state._current->thread->thread;
			state._current->thread->thread.stopped_status_pending = false;
			long unsigned int pid = inferior.id();
			long unsigned int tid = thread.id();
			Genode::log("vStopped: current thread: ", pid, ".", tid);
			gdb_response(out, [&] (Output &out) {
				print(out, "T", Gdb_hex((uint8_t)thread.stop_reason),
				           "thread:p", Gdb_hex(pid), ".", Gdb_hex(tid), ";");
			});
			return;
		}

		state.inferiors.for_each<Inferior_pd const &>([&] (Inferior_pd const &inferior) {
			inferior.for_each_thread([&] (Monitored_thread &thread) {

				if (handled)
					return;

				if (!thread.stopped_status_pending)
					return;

				thread.stopped_status_pending = false;
				long unsigned int pid = inferior.id();
				long unsigned int tid = thread.id();
				Genode::log("vStopped: found stopped thread: ", pid, ".", tid);
				gdb_response(out, [&] (Output &out) {
					print(out, "T", Gdb_hex((uint8_t)thread.stop_reason),
					           "thread:p", Gdb_hex(pid), ".", Gdb_hex(tid), ";");
				});

				handled = true;
			});
		});

		if (!handled) {
			state.notification_in_progress = false;
			gdb_ok(out);
		}
	}
};


/**
 * Resume the inferior
 */
struct vCont : Command_without_separator
{
	vCont(Commands &c) : Command_without_separator(c, "vCont") { }

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("vCont command args: ", Cstring(args.start, args.num_bytes));

		bool handled = false;

		with_skipped_prefix(args, "?", [&] (Const_byte_range_ptr const &) {

			gdb_response(out, [&] (Output &out) {
				print(out, "vCont;c;t"); });

			handled = true;
		});

		with_skipped_prefix(args, ";", [&] (Const_byte_range_ptr const &args) {

			for_each_argument(args, Sep { ';' }, [&] (Const_byte_range_ptr const &arg) {
				log("vCont argument length: ", arg.num_bytes);
				log("vCont argument: ", Cstring(arg.start, arg.num_bytes));

				with_skipped_prefix(arg, "t", [&] (Const_byte_range_ptr const &arg) {

					log("vCont argument t: ", Cstring(arg.start, arg.num_bytes));
					int pid = -1;
					int tid = -1;

					with_skipped_prefix(arg, ":", [&] (Const_byte_range_ptr const &arg) {
						thread_id(arg, pid, tid); });

					log("vCont;t: pid: ", pid, ", tid: ", tid);

					if ((pid == -1) && state._current.constructed())
						pid = (int)state._current->pd.id();

					handled = true;

					state.inferiors.for_each<Inferior_pd const &>([&] (Inferior_pd const &inferior) {

						if (inferior.id() != (unsigned)pid)
							return;

						inferior.for_each_thread([&] (Monitored_thread &thread) {

							if ((tid != -1) && (thread.id() != (unsigned)tid))
								return;

							if (!thread.stopped) {
								thread.pause();
								if (!state.notification_in_progress) {
									state.notification_in_progress = true;
									thread.stopped_status_pending = false;
									gdb_notification(out, [&] (Output &out) {
										print(out, "Stop:T",
										           Gdb_hex((uint8_t)thread.stop_reason),
										           "thread:p",
										           Gdb_hex(inferior.id()),
										           ".",
										           Gdb_hex(thread.id()),
										           ";");
									});
								}
							}
						});
					});
				});

				with_skipped_prefix(arg, "c", [&] (Const_byte_range_ptr const &arg) {

					log("vCont argument c: ", Cstring(arg.start, arg.num_bytes));
					int pid = -1;
					int tid = -1;

					with_skipped_prefix(arg, ":", [&] (Const_byte_range_ptr const &arg) {
						thread_id(arg, pid, tid); });

					log("vCont;c: pid: ", pid, ", tid: ", tid);

					if ((pid == -1) && state._current.constructed())
						pid = (int)state._current->pd.id();

					handled = true;

					state.inferiors.for_each<Inferior_pd const &>([&] (Inferior_pd const &inferior) {

						if (inferior.id() != (unsigned)pid)
							return;

						inferior.for_each_thread([&] (Monitored_thread &thread) {

							if ((tid != -1) && (thread.id() != (unsigned)tid))
								return;

							if (thread.stopped)
								thread.resume();
						});
					});
				});

			});
		});

		if (handled) {
			gdb_ok(out);
			return;
		}

		warning("GDB ", name, " command unsupported: ", Cstring(args.start, args.num_bytes));
	}
};


/**
 * Read value of register
 */
struct p : Command_without_separator
{
	p(Commands &c) : Command_without_separator(c, "p") { }

	void execute(State &/*state*/, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("p command args: ", Cstring(args.start, args.num_bytes));

		/* currently not supported */
		gdb_response(out, [&] (Output &) { });
	}
};


/**
 * Write value of register
 */
struct P : Command_without_separator
{
	P(Commands &c) : Command_without_separator(c, "P") { }

	void execute(State &/*state*/, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("P command args: ", Cstring(args.start, args.num_bytes));

		/* currently not supported */
		gdb_response(out, [&] (Output &) { });
	}
};


/**
 * Stop thread(s)
 */
struct vCtrlC : Command_without_separator
{
	vCtrlC(Commands &c) : Command_without_separator(c, "vCtrlC") { }

	void execute(State &state, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("vCtrlC command args: ", Cstring(args.start, args.num_bytes));

		if (state._current.constructed() &&
		    state._current->thread.constructed()) {

			Inferior_pd &inferior = state._current->pd;
		    Monitored_thread &thread = state._current->thread->thread;

			if (!thread.stopped) {
				thread.pause();
				if (!state.notification_in_progress) {
					state.notification_in_progress = true;
					thread.stopped_status_pending = false;
					gdb_notification(out, [&] (Output &out) {
						print(out, "Stop:T",
					               Gdb_hex((uint8_t)thread.stop_reason),
					               "thread:p",
					               Gdb_hex(inferior.id()),
					               ".",
					               Gdb_hex(thread.id()),
					               ";");
					});
				}
			}
			gdb_ok(out);
			return;
		}

		gdb_error(out, 1);
	}
};

/**
 * File operations
 */
struct vFile : Command_without_separator
{
	vFile(Commands &c) : Command_without_separator(c, "vFile") { }

	void execute(State &, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("vFile command args: ", Cstring(args.start, args.num_bytes));

		/* currently not supported */
		gdb_response(out, [&] (Output &) { });
	}
};


/**
 * Set breakpoint
 */
struct Z : Command_without_separator
{
	Z(Commands &c) : Command_without_separator(c, "Z") { }

	void execute(State &, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("Z command args: ", Cstring(args.start, args.num_bytes));

		/* currently not supported */
		gdb_response(out, [&] (Output &) { });
	}
};


/**
 * Write registers
 */
struct G : Command_without_separator
{
	G(Commands &c) : Command_without_separator(c, "G") { }

	void execute(State &, Const_byte_range_ptr const &args, Output &out) const override
	{
		log("G command args: ", Cstring(args.start, args.num_bytes));

		/* currently not supported */
		gdb_response(out, [&] (Output &) { });
	}
};

} /* namespace Cmd */ } /* namespace Gdb */ } /* namespace Monitor */


/*
 * Registry of all supported commands
 */

namespace Monitor { namespace Gdb { struct Supported_commands; } }

struct Monitor::Gdb::Supported_commands : Commands
{
	template <typename...>
	struct Instances;

	template <typename LAST>
	struct Instances<LAST>
	{
		LAST _last;
		Instances(Commands &registry) : _last(registry) { };
	};

	template <typename HEAD, typename... TAIL>
	struct Instances<HEAD, TAIL...>
	{
		HEAD               _head;
		Instances<TAIL...> _tail;
		Instances(Commands &registry) : _head(registry), _tail(registry) { }
	};

	Instances<
		Cmd::qSupported,
		Cmd::qXfer,
		Cmd::vMustReplyEmpty,
		Cmd::H,
		Cmd::QNonStop,
		Cmd::qSymbol,
		Cmd::qTStatus,
		Cmd::qC,
		Cmd::qAttached,
		Cmd::qOffsets,
		Cmd::g,
		Cmd::m,
		Cmd::D,
		Cmd::T,
		Cmd::ask,
		Cmd::X,
		Cmd::bang,
		Cmd::vStopped,
		Cmd::vCont,
		Cmd::p,
		Cmd::P,
		Cmd::vCtrlC,
		Cmd::vFile,
		Cmd::Z,
		Cmd::G
		> _instances { *this };
};

#endif /* _GDB_STUB_H_ */
