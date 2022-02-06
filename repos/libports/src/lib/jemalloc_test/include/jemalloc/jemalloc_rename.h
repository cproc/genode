/*
 * Name mangling for public symbols is controlled by --with-mangling and
 * --with-jemalloc-prefix.  With default settings the je_ prefix is stripped by
 * these macro definitions.
 */
#ifndef JEMALLOC_NO_RENAME
#  define je_aligned_alloc jealigned_alloc
#  define je_calloc jecalloc
#  define je_dallocx jedallocx
#  define je_free jefree
#  define je_mallctl jemallctl
#  define je_mallctlbymib jemallctlbymib
#  define je_mallctlnametomib jemallctlnametomib
#  define je_malloc jemalloc
#  define je_malloc_conf jemalloc_conf
#  define je_malloc_message jemalloc_message
#  define je_malloc_stats_print jemalloc_stats_print
#  define je_malloc_usable_size jemalloc_usable_size
#  define je_mallocx jemallocx
#  define je_smallocx_ea6b3e973b477b8061e0076bb257dbd7f3faa756 jesmallocx_ea6b3e973b477b8061e0076bb257dbd7f3faa756
#  define je_nallocx jenallocx
#  define je_posix_memalign jeposix_memalign
#  define je_rallocx jerallocx
#  define je_realloc jerealloc
#  define je_sallocx jesallocx
#  define je_sdallocx jesdallocx
#  define je_xallocx jexallocx
#  define je_memalign jememalign
#  define je_valloc jevalloc
#endif
