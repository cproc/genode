/*
 * \brief  Connection to timer service
 * \author Norman Feske
 * \date   2008-08-22
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__TIMER_SESSION__CONNECTION_H_
#define _INCLUDE__TIMER_SESSION__CONNECTION_H_

#include <timer_session/client.h>
#include <base/connection.h>

extern "C" int raw_write_str(char const *str);
extern "C" void wait_for_continue();

#define PRAW(fmt, ...)                                             \
	do {                                                           \
		char str[128];                                             \
		Genode::snprintf(str, sizeof(str),                         \
		                 ESC_ERR fmt ESC_END "\n", ##__VA_ARGS__); \
		raw_write_str(str);                                        \
	} while (0)

namespace Timer {

	class Connection : public Genode::Connection<Session>, public Session_client
	{
		private:

			Lock                      _lock;
			Signal_receiver           _sig_rec;
			Signal_context            _default_sigh_ctx;
			Signal_context_capability _default_sigh_cap;
			Signal_context_capability _custom_sigh_cap;

		public:

			Connection()
			:
				Genode::Connection<Session>(session("ram_quota=8K")),
				Session_client(cap()),
				_default_sigh_cap(_sig_rec.manage(&_default_sigh_ctx))
			{
				PRAW("Timer::Connection: fd = %d", cap().dst().socket);
				PRAW("Timer::Connection: registering signal handler");
				/* register default signal handler */
				Session_client::sigh(_default_sigh_cap);
				PRAW("Timer::Connection: registered signal handler");
			}

			~Connection()
			{
				PRAW("~Timer::Connection() called");
				_sig_rec.dissolve(&_default_sigh_ctx);
				PRAW("~Timer::Connection() finished");
				//wait_for_continue();
			}

			/*
			 * Intercept 'sigh' to keep track of customized signal handlers
			 */
			void sigh(Signal_context_capability sigh)
			{
				_custom_sigh_cap = sigh;
				Session_client::sigh(_custom_sigh_cap);
			}

			void usleep(unsigned us)
			{
				/* serialize sleep calls issued by different threads */
				Lock::Guard guard(_lock);

				/* temporarily install to the default signal handler */
				if (_custom_sigh_cap.valid())
					Session_client::sigh(_default_sigh_cap);

				/* trigger timeout at default signal handler */
				trigger_once(us);
				_sig_rec.wait_for_signal();

				/* revert custom signal handler if registered */
				if (_custom_sigh_cap.valid())
					Session_client::sigh(_custom_sigh_cap);
			}

			void msleep(unsigned ms)
			{
				usleep(1000*ms);
			}
	};
}

#endif /* _INCLUDE__TIMER_SESSION__CONNECTION_H_ */
