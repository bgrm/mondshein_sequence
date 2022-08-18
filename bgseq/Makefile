CPP = g++ -std=c++17

main: inter.o vertex.o bgedge.o chain.o cluster.o bonus.o decom.o main.cpp
	$(CPP) $^ -o $@

decom.o: decom.cpp
	$(CPP) -c $^ -o $@

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


test: runner.sh
	@ make runner
	./runner 10
	
clean:
	rm -f *.o
