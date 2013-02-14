SRC_CC   = env.cc context_area.cc reload_parent_cap.cc debug.cc
LIBS     = ipc heap log_console lock
INC_DIR += $(BASE_DIR)/src/base/env

vpath debug.cc $(REP_DIR)/src/base/env
vpath     %.cc $(BASE_DIR)/src/base/env

