TARGET   = test-ada
REQUIRES = gnat
INC_DIR += $(PRG_DIR)
SRC_ADA  = main.adb
SRC_CC   = add.cc startup.cc
LIBS     = base
