CPP = g++ --std=c++17

main: main.cpp brut.o paths.a
	$(CPP) $^ -o $@

paths.a: element.o micro.o mezzo.o path.o ops.o pathDivision.o microSplitter.o
	ar rcs $@ $^

element.o: element.cpp
	$(CPP) -c $^ -o $@

micro.o: micro.cpp
	$(CPP) -c $^ -o $@

mezzo.o: mezzo.cpp
	$(CPP) -c $^ -o $@

path.o: path.cpp
	$(CPP) -c $^ -o $@

ops.o: ops.cpp
	$(CPP) -c $^ -o $@

pathDivision.o: pathDivision.cpp
	$(CPP) -c $^ -o $@

microSplitter.o: microSplitter.cpp
	$(CPP) -c $^ -o $@


brut.o: brut.cpp
	$(CPP) -c $^ -o $@


clean:
	rm -f *.o *.a *.so main test