/*
 * \brief  Test
 * \author 
 * \date   
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/component.h>

extern void catcher();

void Component::construct(Genode::Env & env)
{
	catcher();
}
