#!/bin/bash

make

for i in {0..100000}; do				
	./test_random $i
	
	if [ $? -ne 0 ]; then
		echo wa on 'test' $i;
		break
	fi
done

