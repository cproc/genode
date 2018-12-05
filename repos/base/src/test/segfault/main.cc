/*
 * \brief  Trigger segmentation fault and test detection by sanitizer
 * \author Norman Feske
 * \date   2012-11-01
 */

#include <base/component.h>
#include <base/log.h>

extern void sanitizer_init(Genode::Env &);

void Component::construct(Genode::Env &env)
{
	env.exec_static_constructors();
	sanitizer_init(env);

	Genode::log("going to produce a segmentation fault...");
	
	int *ptr = nullptr;

	*ptr = 0x55;
}
