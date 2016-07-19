#!/bin/sh

grep '\[init -> sampling_cpu_service\] 0' $1 | sed -e 's/^\[.*\] //' > addresses.txt
