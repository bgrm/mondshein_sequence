#!/bin/bash
g++ -std=c++17 gen.cpp -o gen
make main
for i in {0..10000}; do				
	echo 'gening test' $i
	./gen $1 $i > in.txt				
	echo 'running test' $i
	./main < in.txt
    if [[ $? -ne 0 ]]; then
        break
    fi
done

