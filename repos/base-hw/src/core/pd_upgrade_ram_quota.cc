/*
 * \brief  Core implementation of the PD session interface
 * \author Norman Feske
 * \date   2016-01-13
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* core-local includes */
#include <pd_session_component.h>

using namespace Genode;


void Pd_session_component::session_quota_upgraded()
{
	_pd.upgrade_slab(_sliced_heap);
}

