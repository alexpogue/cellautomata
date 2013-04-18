all: showgen #test

showgen: showgen.o GameGrid.o GridSquare.o Point.o Rect.o
	g++ -o showgen showgen.o GameGrid.o GridSquare.o Point.o Rect.o

showgen.o: showgen.cpp GameGrid.h Rect.h Point.h
	g++ -g -Wall -pedantic -c showgen.cpp

GameGrid.o: GameGrid.cpp GameGrid.h GridSquare.h Point.h Rect.h
	g++ -g -Wall -pedantic -c GameGrid.cpp

GridSquare.o: GridSquare.cpp GridSquare.h Point.h
	g++ -g -Wall -pedantic -c GridSquare.cpp

Point.o: Point.cpp Point.h
	g++ -g -Wall -pedantic -c Point.cpp

Rect.o: Rect.cpp Rect.h Point.h
	g++ -g -Wall -pedantic -c Rect.cpp

AutPreprocessor.o: AutPreprocessor.cpp AutPreprocessor.h
	g++ -g -Wall -pedantic -c AutPreprocessor.cpp

test: TestMain.o AutPreprocessorTest.o AutPreprocessor.o AutParserTest.o AutParser.o Rect.o Point.o GameGrid.o GridSquare.o
	g++ -o test TestMain.o AutPreprocessorTest.o AutPreprocessor.o AutParserTest.o AutParser.o Rect.o Point.o GameGrid.o GridSquare.o -lboost_unit_test_framework

TestMain.o: TestMain.cpp
	g++ -g -Wall -pedantic -c TestMain.cpp

AutPreprocessorTest.o: AutPreprocessorTest.cpp AutPreprocessor.h
	g++ -g -Wall -pedantic -c AutPreprocessorTest.cpp

AutParser.o: AutParser.cpp AutParser.h GameGrid.h
	g++ -g -Wall -pedantic -c AutParser.cpp

AutParserTest.o: AutParserTest.cpp AutParser.h GameGrid.h Rect.h Point.h
	g++ -g -Wall -pedantic -c AutParserTest.cpp

clean:
	rm *.o

project.tar.gz: makefile showgen.cpp AutParser.cpp AutParser.h AutParserTest.cpp AutPreprocessor.cpp AutPreprocessor.h AutPreprocessorTest.cpp GameGrid.cpp GameGrid.h GridSquare.cpp GridSquare.h Point.cpp Point.h Rect.cpp Rect.h TestMain.cpp
	tar -czf project.tar.gz makefile showgen.cpp AutParser.cpp AutParser.h AutParserTest.cpp AutPreprocessor.cpp AutPreprocessor.h AutPreprocessorTest.cpp GameGrid.cpp GameGrid.h GridSquare.cpp GridSquare.h Point.cpp Point.h Rect.cpp Rect.h TestMain.cpp
