
#include <base/printf.h>
#include <base/sleep.h>
#include <dataspace/client.h>
#include <dataspace/capability.h>
#include <ram_session/connection.h>
#include <cpu_session/connection.h>

#define REDUCE_KB(X) (X >> 10)
#define REDUCE_MB(X) (X >> 20)
#define REDUCE_GB(X) (X >> 30)

#define KB(X) (X << 10)
#define MB(X) (X << 20)
#define GB(X) (X << 30)

using namespace Genode;

int main()
{
  PLOG("Large memory issue test #1");
  PLOG("env()->Ram_session() available: %lu MB", REDUCE_MB(env()->ram_session()->avail()));
  PLOG("env()->ram_session() quota: %lu MB", REDUCE_MB(env()->ram_session()->quota()));

  try {
    Ram_connection ram;
    ram.ref_account(env()->ram_session_cap());

    env()->parent()->upgrade(ram.cap(), "ram_quota=1M");

    size_t process_avail = env()->ram_session()->avail();
    //size_t alloc_size = MB(550); //this will be ok
    size_t alloc_size = MB(600); //this will fail
    if(alloc_size > process_avail) {
      PERR("Not enough memory available.");
      return -1;
    }

    env()->ram_session()->transfer_quota(ram.cap(),alloc_size+4096);
    Ram_dataspace_capability ds = ram.alloc(alloc_size);
    if(!ds.valid()) {
      PERR("Something went wrong with ram.alloc");
      return -1;
    }
    
    Dataspace_client dsc(ds);
    PLOG("Phys alloc: 0x%p-0x%p (%lu MB)",
         (void*) dsc.phys_addr(), 
         (void*) (dsc.phys_addr()+dsc.size()),
         REDUCE_MB(dsc.size()));

    /* attach VM space */
    void * addr = (void*) env()->rm_session()->attach(ds);
    //void * addr = (void*) env()->rm_session()->attach_at(ds,0xBB00000000ULL);
    PLOG("VM addr:    0x%p",addr);
    
    PLOG("About to memset memory area");
    __builtin_memset(addr,0xee,alloc_size);
    PLOG("Memset passed OK.(%lu MB)",REDUCE_MB(alloc_size));
  }
  catch(...) {
    PERR("Arrggh. Unhandled exception.");
  }

  Genode::sleep_forever();
}

