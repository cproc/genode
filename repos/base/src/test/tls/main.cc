/*
 * \brief  Test TLS support
 * \author Christian Prochaska
 * \date   2019-06-13
 */

#include <base/component.h>
#include <base/log.h>
#include <base/thread.h>


static thread_local int thread_local_x;
static thread_local int thread_local_y;


struct Test_thread : Genode::Thread
{
	int _thread_id;
	int _x;
	int _y;

	Test_thread(Genode::Env &env, int thread_id, int x, int y)
	: Genode::Thread(env, "test_thread", 16*1024),
	  _thread_id(thread_id), _x(x), _y(y)
	{
		start();
	}

	void entry() override
	{
		thread_local_x = _x;
		thread_local_y = _y;
		Genode::log("thread ", _thread_id,
		            ": x: ", thread_local_x,
		            ", y: ", thread_local_y);
	}
};


void Component::construct(Genode::Env &env)
{
	using namespace Genode;

	thread_local_x = 4;
	thread_local_y = 5;

	for (int i = 0; i < 2; i++) {
		Test_thread test_thread(env, i, (i * 2), (i * 2) + 1);
		test_thread.join();
	}

	Genode::log("main:     x: ", thread_local_x,
	            ", y: ", thread_local_y);
}
