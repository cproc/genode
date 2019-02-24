/*
 * \brief  Interface for querying information about the depot
 * \author Norman Feske
 * \date   2019-02-22
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _DEPOT_QUERY_H_
#define _DEPOT_QUERY_H_

#include "types.h"

namespace Sculpt { struct Depot_query; }


struct Sculpt::Depot_query : Interface
{
	struct Version { unsigned value; };

	virtual Version depot_query_version() const = 0;

	virtual void trigger_depot_query() = 0;
};

#endif /* _DEPOT_QUERY_H_ */
