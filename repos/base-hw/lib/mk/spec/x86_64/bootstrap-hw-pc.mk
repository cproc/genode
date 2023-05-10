REP_INC_DIR += src/bootstrap/spec/x86_64

SRC_S   += bootstrap/spec/x86_64/crt0.s
SRC_CC  += bootstrap/spec/x86_64/platform.cc
SRC_S   += bootstrap/spec/x86_64/crt0_translation_table.s

SRC_CC  += hw/spec/64bit/memory_map.cc

CC_MARCH = -m64 -mcmodel=large -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx

include $(call select_from_repositories,lib/mk/bootstrap-hw.inc)
