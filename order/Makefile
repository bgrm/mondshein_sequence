CPP = g++ -std=c++17

test: test.cpp order.o sqrt.o
	$(CPP) $^ -o $@
	
order.o: order.cpp
	$(CPP) -c $^ -o $@

sqrt.o: sqrt.cpp
	$(CPP) -c $^ -o $@

clean:
	rm -f *.o *.out *.txt test
