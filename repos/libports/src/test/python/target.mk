TARGET   = test-python
LIBS     = base python posix
REQUIRES = x86
SRC_CC   = main.cc

# disable C++17 warnings about 'register' keyword
CC_CXX_WARN += -Wno-register

CC_CXX_WARN_STRICT =
