#!/bin/sh

grep '\[init -> sampling_cpu_service\] 0' log.txt | sed -e 's/^\[.*\] //' > addresses.txt
