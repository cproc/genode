/*
 * \brief   Arndale specific definitions for 'native_types' header
 * \author  Sebastian Sumpf
 * \date    2013-02-12
 */


#include <base/native_types.h>

using namespace Genode;

addr_t Native_config::context_area_virtual_base() { return 0x20000000UL; }
