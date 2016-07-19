#!/bin/sh

# usage: generate_statistics.sh <binary file> <file containing the shared library load addresses> <file containing the sampled addresses>


cat $2 $3 > ld_plus_addresses.txt
./addr_to_sym $1 ld.lib.so < ld_plus_addresses.txt | sed -e '/Scanned image/d' > symbols.txt
cat symbols.txt | sort | uniq --count | sort -rn > symbols_counted.txt

echo "symbols_counted.txt generated"

cat $3 | sort | uniq --count | sort -rn > addresses_sorted_and_counted.txt
cat addresses_sorted_and_counted.txt | awk '{print $2}' > addresses_sorted_and_counted_without_count.txt

cat $2 addresses_sorted_and_counted_without_count.txt > ld_plus_addresses_sorted_and_counted_without_count.txt
backtrace $1 ld.lib.so < ld_plus_addresses_sorted_and_counted_without_count.txt | sed -e '/Scanned image/d' > locations.txt
