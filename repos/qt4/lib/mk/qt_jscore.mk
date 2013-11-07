include $(REP_DIR)/lib/import/import-qt_jscore.mk

SHARED_LIB = yes

# additional defines for the Genode version
CC_OPT += -DSQLITE_NO_SYNC=1 -DSQLITE_THREADSAFE=0

# enable C++ functions that use C99 math functions (disabled by default in the Genode tool chain)
CC_CXX_OPT += -D_GLIBCXX_USE_C99_MATH

# use default warning level to avoid noise when compiling contrib code
CC_WARN =

# some parts of the library are not C++11 compatible
CC_CXX_OPT_STD =

# StackBounds includes Genode headers, which rely on C++11. Hence, we need
# to selectively enable C++11 for this compilation unit.
CC_OPT_StackBounds = -std=gnu++11

include $(REP_DIR)/lib/mk/qt_jscore_generated.inc

include $(REP_DIR)/lib/mk/qt.inc

LIBS += qt_network qt_core libc libm
