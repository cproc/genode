/* Genode includes */
#include <base/component.h>
#include <base/thread.h>

using namespace Genode;

struct Test_thread : Thread
{
	Test_thread(Env &env) : Thread(env, "thread", 8192) { }

	void entry() override
	{
		for (;;);
	}
};


void Component::construct(Env &env)
{
	Test_thread t(env);
	t.start();

	for (;;);
}
