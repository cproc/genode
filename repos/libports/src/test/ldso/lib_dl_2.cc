/*
 * \brief  Test for the use of 'Shared_object' API
 * \author Sebastian Sumpf
 * \author Norman Feske
 * \date   2014-05-20
 */

/*
 * Copyright (C) 2014-2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/log.h>

/* test-local includes */
#include "test-ldso.h"

using namespace Genode;

extern "C" void lib_dl_2_test()
{
	log(__func__, ": call 'lib_1_good':");
	lib_1_good();
	log(__func__, ": call 'lib_dl_1_good':");
	lib_dl_1_good();
}
