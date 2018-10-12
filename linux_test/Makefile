test: main.o func/func.o func2/func.o
	gcc -fprofile-arcs -ftest-coverage -fprofile-dir=$(CURDIR)/gcov -o $@ $^

main.o: main.c
	gcc -fprofile-arcs -ftest-coverage -fprofile-dir=$(CURDIR)/gcov -o $@ -c $(abspath $<)

func/func.o: func/func.c
	gcc -fprofile-arcs -ftest-coverage -fprofile-dir=$(CURDIR)/gcov -o $@ -c $(abspath $<)

func2/func.o: func2/func.c
	gcc -fprofile-arcs -ftest-coverage -fprofile-dir=$(CURDIR)/gcov -o $@ -c $(abspath $<)
