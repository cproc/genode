#!/bin/sh

grep '\[init -> sampling_cpu_service\] 0' log.txt | sed -e 's/^\[.*\] //' | sort | uniq --count | sort -rn > log2.txt
cat log2.txt | awk '{print $2}' > log3.txt
