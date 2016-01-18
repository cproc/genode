/*
 * \brief  Thread_data interface 
 * \author Christian Prochaska
 * \date   2016-01-19
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _THREAD_DATA_H_
#define _THREAD_DATA_H_

/* Genode includes */
#include <base/thread.h>
#include <util/list.h>

/* local includes */
#include "cpu_session_component.h"

namespace Sampling_cpu_service
{
	using namespace Genode;
	class Thread_data;
	class Cpu_session_component;
}

class Sampling_cpu_service::Thread_data : public Genode::List<Thread_data>::Element
{
	private:

		enum {
			LABEL_LEN = 64,
			SAMPLE_BUF_SIZE = 4096,
			SAMPLE_SIZE = 16 + 1, /* 16 hex characters + newline */
		};

		Cpu_session_component &_cpu_session_component;
		Thread_capability      _thread_cap;
		
		char                   _label[LABEL_LEN];

		char                   _sample_buf[SAMPLE_BUF_SIZE];
		unsigned int           _sample_buf_index = 0;

		int                    fd;

	public:

		Thread_data(Cpu_session_component &cpu_session_component,
		            Thread_capability thread_cap,
	    	        const char *thread_name);

		Thread_capability const thread_cap() const { return _thread_cap; }
		char const *label() { return _label; }

		void take_sample();
		void reset();
		void flush();

};

#endif /* _THREAD_DATA_H_ */
