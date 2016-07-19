#!/bin/sh

grep '\[init -> sampling_cpu_service -> samples ->' $1 | sed -e 's/^\[.*\] //' > addresses.txt
