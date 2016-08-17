TARGET  = sampling_cpu_service

SRC_CC += main.cc \
          cpu_session_component.cc \
          cpu_thread_component.cc

INC_DIR = $(REP_DIR)/src/server/sampling_cpu_service

LIBS    = base

vpath %.cc $(REP_DIR)/src/server/sampling_cpu_service
