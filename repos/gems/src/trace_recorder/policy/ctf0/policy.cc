#include <util/string.h>
#include <base/trace/policy.h>
#include <trace/policy.h>
#include <trace_recorder_policy/ctf_stream0.h>

using namespace Genode;
using namespace Ctf;

//static bool rpc_call_enabled = false;

enum { MAX_EVENT_SIZE = 256 };

size_t max_event_size()
{
	return MAX_EVENT_SIZE;
}

size_t trace_eth_packet(char *, char const *, bool, char *, size_t)
{
	return 0;
}

size_t checkpoint(char *dst, char const *name, unsigned long data, void *addr, unsigned char type)
{
#if 0
	return 0;
#endif
#if 1
	size_t len = strlen(name) + 1;

	size_t total_len = len + sizeof(Checkpoint);

	if (total_len > max_event_size()) {
		new (dst) Checkpoint("error", 6, total_len, nullptr, 0);
		return 6 + sizeof(Checkpoint);
	}

	new (dst) Checkpoint(name, len, data, addr, type);

	return total_len;
#endif
}

size_t lock_locked(char *dst, void const *lock)
{
	new (dst) Lock_locked(lock);

	return sizeof(Lock_locked);
}

size_t lock_wait(char *dst, void const *lock, char const *owner, char const *name)
{
	size_t owner_len = strlen(owner) + 1;
	size_t name_len = strlen(name) + 1;

	new (dst) Lock_wait(lock, owner, owner_len, name, name_len);

	return owner_len + name_len + sizeof(Lock_wait);
}

size_t lock_unlock(char *dst, void const *lock, char const *next_owner)
{
	size_t len = strlen(next_owner) + 1;

	new (dst) Lock_unlock(lock, next_owner, len);

	return len + sizeof(Lock_unlock);
}

size_t log_output(char *dst, char const *log_message, size_t len) {
	return 0;
}

size_t rpc_call(char *dst, char const *rpc_name, Msgbuf_base const &)
{
#if 0
	return 0;
#endif
#if 1
	size_t len = strlen(rpc_name) + 1;

	new (dst) Rpc_call(rpc_name, len);

	return len + sizeof(Rpc_call);
#endif
}

size_t rpc_returned(char *dst, char const *rpc_name, Msgbuf_base const &)
{
#if 0
	return 0;
#endif
#if 1
	size_t len = strlen(rpc_name) + 1;

	new (dst) Rpc_returned(rpc_name, len);

	return len + sizeof(Rpc_returned);
#endif
}

size_t rpc_dispatch(char *dst, char const *rpc_name)
{
#if 0
	return 0;
#endif
#if 1
	size_t len = strlen(rpc_name) + 1;

	new (dst) Rpc_dispatch(rpc_name, len);

	return len + sizeof(Rpc_dispatch);
#endif
}

size_t rpc_reply(char *dst, char const *rpc_name)
{
#if 0
	return 0;
#endif
#if 1
	size_t len = strlen(rpc_name) + 1;

	new (dst) Rpc_reply(rpc_name, len);

	return len + sizeof(Rpc_reply);
#endif
}

size_t signal_submit(char *dst, unsigned const num)
{
#if 0
	return 0;
#endif
#if 1
	new (dst) Signal_submit(num);

	return sizeof(Signal_submit);
#endif
}

size_t signal_receive(char *dst, Signal_context const & context, unsigned num)
{
#if 0
	return 0;
#endif
#if 1
	new (dst) Signal_receive(num, (void*)&context);
	return 0;
#endif
}
#if 0
void enable_rpc_call()
{
	rpc_call_enabled = true;
}
#endif
