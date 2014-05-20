REQUIRES = arm

include $(REP_DIR)/lib/mk/linker.inc

INC_DIR += $(DIR)/arm
vpath %.s $(DIR)/arm
