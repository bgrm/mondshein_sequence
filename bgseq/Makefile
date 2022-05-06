CPP = g++ -std=c++17

decom: debug.o inter.o vertex.o bgedge.o chain.o cluster.o bonus.o decom.cpp
	$(CPP) $^ -o $@

inter.o: inter.cpp
	$(CPP) -c $^ -o $@
	
vertex.o: vertex.cpp
	$(CPP) -c $^ -o $@
	
bgedge.o: bgedge.cpp
	$(CPP) -c $^ -o $@
	
chain.o: chain.cpp
	$(CPP) -c $^ -o $@
	
cluster.o: cluster.cpp
	$(CPP) -c $^ -o $@

bonus.o: bonus.cpp
	$(CPP) -c $^ -o $@

debug.o: debug.cpp
	$(CPP) -c $^ -o $@

	
clean:
	rm -f *.o
