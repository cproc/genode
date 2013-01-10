/*
 * \brief  ROM-session capability type
 * \author Norman Feske
 * \date   2008-08-16
 */

/*
 * Copyright (C) 2008-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__ROM_SESSION__CAPABILITY_H_
#define _INCLUDE__ROM_SESSION__CAPABILITY_H_

#include <base/capability.h>
#include <rom_session/rom_session.h>

namespace Genode { typedef Capability<Rom_session> Rom_session_capability; }

#endif /* _INCLUDE__ROM_SESSION__CAPABILITY_H_ */
