TARGET   = java
SRC_C    = main.c
LIBS     = jtool jli java jvm libc

CC_C_OPT = -DVERSION_STRING='"9-genode.openjdk"' -D__GENODE__

JDK_PATH = $(call select_from_ports,jdk)/src/app/jdk
vpath main.c $(JDK_PATH)/jdk/src/java.base/share/native/launcher
