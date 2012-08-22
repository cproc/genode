/*
 * \brief  Process environment utility
 * \author Norman Feske
 * \date   2011-02-17
 */

/*
 * Copyright (C) 2011-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <util/string.h>
#include <util/arg_string.h>
#include <os/attached_ram_dataspace.h>
#include <base/printf.h>

/* Noux includes */
#include <path.h>
#include <range_checked_index.h>

namespace Noux {

	/**
	 * Front-end for PWD environment variable
	 */
	class Environment : private Attached_ram_dataspace
	{
		private:

			enum { ENV_DS_SIZE = 4096 };

			char *_env;

		public:

			/**
			 * \param env  comma-separated list of environment variables
			 */
			Environment(char const *env) :
				Attached_ram_dataspace(Genode::env()->ram_session(), ENV_DS_SIZE),
				_env(local_addr<char>())
			{
				strncpy(_env, env, ENV_DS_SIZE);
			}

			using Attached_ram_dataspace::cap;

			/**
			 * Return list of environment variables as comma-separated list
			 */
			char const *env() { return _env; }
	};
}
