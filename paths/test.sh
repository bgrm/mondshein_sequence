#!/bin/bash
#argument $1 - number of operations to perform

make main

for i in {0..10000}; do			
	echo 'running test on seed' $i'...'
	./main $i $1
    if [[ $? -ne 0 ]]; then
        break
    fi
done

