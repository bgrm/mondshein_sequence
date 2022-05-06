#!/bin/bash
g++ -std=c++17 gen.cpp -o gen
make decom
#make gen
for i in {0..1000}; do				
	echo 'gening test' $i
	./gen $1 $i > in.txt				
	echo 'running test' $i
	./decom < in.txt
    if [[ $? -ne 0 ]]; then
        break
    fi
done

