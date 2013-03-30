all: showgen

showgen: showgen.o
	g++ -o showgen showgen.o

showgen.o: showgen.cpp
	g++ -Wall -pedantic -c showgen.cpp

clean:
	rm *.o

project.tar.gz: makefile showgen.cpp
	tar -czf project.tar.gz makefile showgen.cpp
