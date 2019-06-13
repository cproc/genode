/*
 * \brief  Test TLS support
 * \author Christian Prochaska
 * \date   2019-06-13
 */

#include <base/component.h>
#include <base/log.h>
#include <base/thread.h>


static thread_local int x;
static thread_local int y;


struct Test_thread : Genode::Thread
{
	Test_thread(Genode::Env &env)
	: Genode::Thread(env, "test_thread", 16*1024)
	{
		start();
	}

	void entry() override
	{
		x = 2;
		y = 4;
		for (int i = 0; i < 10; i++)
			Genode::log("thread: x: ", x, ", y: ", y);
	}
};


void Component::construct(Genode::Env &env)
{
	using namespace Genode;

	x = 1;
	y = 3;

	Test_thread test_thread(env);

	for (int i = 0; i < 10; i++)
		Genode::log("main:   x: ", x, ", y: ", y);
}
