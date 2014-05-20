REQUIRES = x86 32bit

include $(REP_DIR)/lib/mk/linker.inc

INC_DIR += $(DIR)/x86_32
vpath %.s $(DIR)/x86_32
