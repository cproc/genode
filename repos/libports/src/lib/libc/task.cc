/*
 * \brief  User-level task based libc
 * \author Christian Helmuth
 * \date   2016-01-22
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/component.h>
#include <base/printf.h>
#include <base/thread.h>
#include <base/rpc_server.h>
#include <base/rpc_client.h>

/* libc-internal includes */
#include <internal/call_func.h>


#define P(fmt,...)                                                    \
	do {                                                              \
		int dummy;                                                    \
		Genode::printf(ESC_INF "[%lx] %s:%u " fmt ESC_END "\n",       \
		               (unsigned long)&dummy >> 20,                   \
		               __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__); \
	} while (0)


namespace Libc {
	class Task;

	void (*original_call_component_construct)(Genode::Environment &);
	void call_component_construct(Genode::Environment &env);
}


struct Task_resume
{
	GENODE_RPC(Rpc_resume, void, resume);
	GENODE_RPC_INTERFACE(Rpc_resume);
};


/**
 * Libc task
 *
 * The libc task represents the "kernel" of the libc-based application.
 * Blocking and deblocking happens here on libc functions like read() or
 * select(). This combines blocking of the VFS backend and other signal sources
 * (e.g., timers). The libc task runs on the component thread and allocates a
 * secondary stack for the application task. Context switching uses
 * setjmp/longjmp.
 */
class Libc::Task : public Genode::Rpc_object<Task_resume, Libc::Task>
{
	private:

		Genode::Environment &_env;

		/**
		 * Application context and execution state
		 */
		bool    _app_runnable = true;
		jmp_buf _app_task;

		void *_app_stack = {
			Genode::Thread_base::myself()->alloc_secondary_stack(Component::name(),
			                                                     Component::stack_size()) };

		/**
		 * Libc context
		 */
		jmp_buf _libc_task;

		/**
		 * Trampoline to application code
		 */
		static void _app_entry(Task *);

		/* executed in the context of the main thread */
		static void _resumed_callback();

	public:

		Task(Genode::Environment &env) : _env(env) { }

		void run()
		{
			/* save continuation of libc task (incl. current stack) */
			if (!_setjmp(_libc_task)) {
				/* _setjmp() returned directly -> switch to app stack and launch component */
				call_func(_app_stack, (void *)_app_entry, (void *)this);

				/* never reached */
			}

			/* _setjmp() returned after _longjmp() -> we're done */
		}

		/**
		 * Called in the context of the entrypoint via RPC
		 */
		void resume()
		{
			if (!_setjmp(_libc_task))
				_longjmp(_app_task, 1);
		}

		/**
		 * Called from the app context (by fork)
		 */
		void schedule_suspend(void(*suspended_callback) ())
		{
			if (_setjmp(_app_task))
				return;

			_env.ep().schedule_suspend(suspended_callback, _resumed_callback);

			/* switch to libc task, which will return to entrypoint */
			_longjmp(_libc_task, 1);
		}

		/**
		 * Called from the context of the initial thread
		 */
		void resumed()
		{
			Genode::Capability<Task_resume> cap = _env.ep().manage(*this);
			cap.call<Task_resume::Rpc_resume>();
			_env.ep().dissolve(*this);
		}
};


/******************************
 ** Libc task implementation **
 ******************************/

void Libc::Task::_app_entry(Task *task)
{
	original_call_component_construct(task->_env);

	/* returned from task - switch stack to libc and return to dispatch loop */
	_longjmp(task->_libc_task, 1);
}


/**
 * Libc task singleton
 */
static Genode::Lazy_volatile_object<Libc::Task> task;


void Libc::Task::_resumed_callback() { task->resumed(); }


namespace Libc {

	void schedule_suspend(void (*suspended) ())
	{
		task->schedule_suspend(suspended);
	}

	void schedule_exit(int status) __attribute__((noreturn));
}


extern void genode_exit(int status) __attribute__((noreturn));

static int exit_status;
static void exit_on_suspend() { genode_exit(exit_status); }

void Libc::schedule_exit(int status)
{
	exit_status = status;
	schedule_suspend(exit_on_suspend);

	PERR("exit_on_suspend should not return");
	while (1) ;
}


/****************************
 ** Component-startup hook **
 ****************************/

/* XXX needs base-internal header? */
namespace Genode { extern void (*call_component_construct)(Genode::Environment &); }

void Libc::call_component_construct(Genode::Environment &env)
{
	task.construct(env);
	task->run();
}


static void __attribute__((constructor)) libc_task_constructor(void)
{
	/* hook into component startup */
	Libc::original_call_component_construct = Genode::call_component_construct;
	Genode::call_component_construct        = &Libc::call_component_construct;
}
