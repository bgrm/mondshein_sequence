CPP = g++ --std=c++17

test_random: paths.o lib.o brut.o test_random.cpp
	$(CPP) $^ -o $@
	
paths.o: paths.cpp
	$(CPP) -c $< -o $@
	
lib.o: lib.cpp
	$(CPP) -c $< -o $@

brut.o: brut.cpp brut.h
	$(CPP) -c $< -o $@ 


clean:
	rm -f *.o test_random

