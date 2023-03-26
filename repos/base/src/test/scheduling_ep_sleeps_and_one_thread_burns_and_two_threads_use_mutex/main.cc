/* Genode includes */
#include <base/component.h>
#include <base/sleep.h>
#include <base/thread.h>

using namespace Genode;


static Mutex &mutex()
{
	static Mutex inst;
	return inst;
}


struct Burner : Thread
{
	Burner(Env &env) : Thread(env, "burner", 8192) { }

	void entry() override
	{
		for (;;);
	}
};


struct Mutex_user : Thread
{
	Mutex_user(Env &env) : Thread(env, "mutex_user", 8192) { }

	void entry() override
	{
		for (;;) {
			mutex().acquire();
			mutex().release();
		}
	}
};


void Component::construct(Env &env)
{
	Burner burner(env);
	burner.start();

	Mutex_user mutex_user(env);
	mutex_user.start();

	Mutex_user mutex_user2(env);
	mutex_user2.start();

	sleep_forever();
}
