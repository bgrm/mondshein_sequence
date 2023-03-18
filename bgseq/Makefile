CPP = g++ -std=c++17

main: main.cpp bgseq.a
	$(CPP) $^ -o $@


bgseq.a: bgedge.o inter.o vertex.o chain.o cluster.o bonus.o decom.o
	ar rcs $@ $^
	
bgedge.o: bgedge.cpp
	$(CPP) -c $^ -o $@

inter.o: inter.cpp
	$(CPP) -c $^ -o $@
	
vertex.o: vertex.cpp
	$(CPP) -c $^ -o $@
	
chain.o: chain.cpp
	$(CPP) -c $^ -o $@
	
cluster.o: cluster.cpp
	$(CPP) -c $^ -o $@

bonus.o: bonus.cpp
	$(CPP) -c $^ -o $@
	
decom.o: decom.cpp
	$(CPP) -c $^ -o $@


clean:
	rm -f *.o *.so *.a main test gen
