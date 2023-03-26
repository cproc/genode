/* Genode includes */
#include <base/component.h>
#include <base/sleep.h>
#include <base/thread.h>

using namespace Genode;

struct Burner : Thread
{
	Burner(Env &env) : Thread(env, "burner", 8192) { }

	void entry() override
	{
		for (;;);
	}
};


void Component::construct(Env &env)
{
	Burner burner(env);
	burner.start();

	Burner burner2(env);
	burner2.start();

	Burner burner3(env);
	burner3.start();

	sleep_forever();
}
