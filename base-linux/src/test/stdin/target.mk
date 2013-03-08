TARGET = test-stdin
LIBS   = base
SRC_CC = main.cc

$(TARGET) : test

test: test.c
	gcc -o $@ $^

vpath test.c $(PRG_DIR)
