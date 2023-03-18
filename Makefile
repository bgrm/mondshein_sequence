CPP = g++ --std=c++17

main: main.cpp mond.o shorts.o order.o bgseq.a paths.a
	$(CPP) $^ -o $@
	

mond.o: mond.cpp
	$(CPP) -c $^ -o $@

shorts.o: shorts.cpp
	$(CPP) -c $^ -o $@

order.o: order/order.cpp
	$(CPP) -c $^ -o $@

sqrt.o: order/sqrt.cpp
	$(CPP) -c $^ -o $@

bgseq.a: bgseq/decom.cpp
	cd bgseq && make $@ && cp $@ .. && cd ..

paths.a: paths/ops.cpp
	cd paths && make $@ && cp $@ .. && cd ..

	
clean:
	rm -f *.o *.a *.so in.txt main gen test 
