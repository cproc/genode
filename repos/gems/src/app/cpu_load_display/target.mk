TARGET   = cpu_load_display
SRC_CC   = main.cc
LIBS     = base
INC_DIR += $(PRG_DIR)

CC_WARN += -Wno-address-of-packed-member
