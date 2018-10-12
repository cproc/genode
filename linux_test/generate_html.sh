#!/bin/sh

lcov --gcov-tool genode-x86-gcov --directory . --capture --output-file test.info
genhtml --demangle-cpp test.info

