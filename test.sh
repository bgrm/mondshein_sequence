#!/bin/bash
g++ -std=c++17 bgseq/gen.cpp -o gen
make
for i in {0..10000}; do				
	#echo 'gening test' $i
	./gen $1 $i 1 > in.txt
	echo 'running test on seed' $i '...'
	./main < in.txt
    if [[ $? -ne 0 ]]; then
       break
    fi
done
