#!/bin/sh

lcov --directory . --capture --output-file test.info
genhtml test.info

