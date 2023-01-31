CPP = g++ --std=c++17

main: paths.o lib.o order.o vertex.o bgedge.o chain.o bonus.o cluster.o inter.o decom.o shorts.o sqrt.o mond.o main.cpp
	$(CPP) $^ -o $@
	
mond.o: mond.cpp
	$(CPP) -c $^ -o $@

paths.o: paths/paths.cpp
	$(CPP) -c $^ -o $@

lib.o: paths/lib.cpp
	$(CPP) -c $^ -o $@

order.o: order/order.cpp
	$(CPP) -c $^ -o $@
	
vertex.o: bgseq/vertex.cpp
	$(CPP) -c $^ -o $@
	
bgedge.o: bgseq/bgedge.cpp
	$(CPP) -c $^ -o $@
	
chain.o: bgseq/chain.cpp
	$(CPP) -c $^ -o $@
	
bonus.o: bgseq/bonus.cpp
	$(CPP) -c $^ -o $@
	
cluster.o: bgseq/cluster.cpp
	$(CPP) -c $^ -o $@

inter.o: bgseq/inter.cpp
	$(CPP) -c $^ -o $@
	
decom.o: bgseq/decom.cpp
	$(CPP) -c $^ -o $@

shorts.o: shorts.cpp
	$(CPP) -c $^ -o $@


sqrt.o: order/sqrt.cpp
	$(CPP) -c $^ -o $@

	
clean:
	rm -f *.o in.txt test 
