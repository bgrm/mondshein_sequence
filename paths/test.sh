#!/bin/bash
#argument 1 specifies number of operations performed in each test

make

for i in {0..10000}; do
	echo testing with seed $i...
	./test_random $i $1
	
	if [ $? -ne 0 ]; then
		echo wa on test $i;
		break
	fi
done

