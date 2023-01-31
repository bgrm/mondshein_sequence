#!/bin/bash
# argument 1 specifies the number of bgoperations used to generate the graphs

if [ $# -eq 0 ]; then
    echo usage: ./test \'number of bg-ops for graph generating\'
	exit 1
fi

g++ -std=c++17 bgseq/gen.cpp -o gen
make

for i in {0..10000}; do
	./gen $1 $i 1 > in.txt
	echo 'running test on seed' $i '...'
	./main < in.txt
    if [[ $? -ne 0 ]]; then
       break
    fi
done
