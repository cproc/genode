TARGET = sampling_cpu_service
SRC_CC = main.cc \
         cpu_session_component.cc \
         thread_data.cc
LIBS   = base config server libc
