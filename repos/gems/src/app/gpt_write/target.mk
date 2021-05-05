TARGET  := gpt_write
LIBS    := base jitterentropy
SRC_CC  := main.cc util.cc
INC_DIR := $(PRG_DIR)

CC_WARN += -Wno-address-of-packed-member
