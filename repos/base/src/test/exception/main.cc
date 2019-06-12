#include <base/component.h>
#include <base/log.h>
#include <base/thread.h>

struct Test_exception { };

static void test_func(bool t)
{
	try {
		if (t) throw Test_exception();
	} catch (Test_exception) { }
}

template <typename T>
struct Test_thread : Genode::Thread
{
	Test_thread(Genode::Env &env)
	: Genode::Thread(env, "test_thread", 16*1024)
	{
		start();
	}

	void entry() override
	{
		for (int i = 0; ; i++) {
#if 0
			try {
				throw T();
			} catch (T) { };
#endif
			test_func(i);
		}
	}
};

struct Thread0_exception { };
struct Thread1_exception { };
struct Thread2_exception { };
struct Thread3_exception { };
struct Thread4_exception { };
struct Thread5_exception { };
struct Thread6_exception { };
struct Thread7_exception { };
struct Thread8_exception { };
struct Thread9_exception { };
struct Thread10_exception { };
struct Thread11_exception { };
struct Thread12_exception { };
struct Thread13_exception { };
struct Thread14_exception { };
struct Thread15_exception { };
struct Thread16_exception { };
struct Thread17_exception { };
struct Thread18_exception { };
struct Thread19_exception { };

void Component::construct(Genode::Env &env)
{
	using namespace Genode;
	
	static Test_thread<Thread0_exception> test_thread0(env);
	static Test_thread<Thread1_exception> test_thread1(env);
	static Test_thread<Thread2_exception> test_thread2(env);
	static Test_thread<Thread3_exception> test_thread3(env);
	static Test_thread<Thread4_exception> test_thread4(env);
	static Test_thread<Thread5_exception> test_thread5(env);
	static Test_thread<Thread6_exception> test_thread6(env);
	static Test_thread<Thread7_exception> test_thread7(env);
	static Test_thread<Thread8_exception> test_thread8(env);
	static Test_thread<Thread9_exception> test_thread9(env);
	static Test_thread<Thread10_exception> test_thread10(env);
	static Test_thread<Thread11_exception> test_thread11(env);
	static Test_thread<Thread12_exception> test_thread12(env);
	static Test_thread<Thread13_exception> test_thread13(env);
	static Test_thread<Thread14_exception> test_thread14(env);
	static Test_thread<Thread15_exception> test_thread15(env);
	static Test_thread<Thread16_exception> test_thread16(env);
	static Test_thread<Thread17_exception> test_thread17(env);
	static Test_thread<Thread18_exception> test_thread18(env);
	static Test_thread<Thread19_exception> test_thread19(env);

}
