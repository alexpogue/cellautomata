all: showgen sim-tui sim-gui #test

showgen: showgen.o GameGrid.o GridSquare.o Point.o Rect.o AutParser.o AutPreprocessor.o GolSimulator.o AutWriter.o CellState.o StateColor.o BbSimulator.o WwSimulator.o LaSimulator.o
	g++ -o showgen showgen.o GameGrid.o GridSquare.o Point.o Rect.o AutParser.o AutPreprocessor.o GolSimulator.o AutWriter.o CellState.o StateColor.o BbSimulator.o WwSimulator.o LaSimulator.o

sim-tui: sim-tui.o GameGrid.o Point.o GridSquare.o CellState.o Rect.o AutWriter.o AutParser.o AutPreprocessor.o TuiDisplay.o GridDisplay.o GolSimulator.o StateColor.o
	g++ -o sim-tui sim-tui.o GameGrid.o Point.o GridSquare.o CellState.o Rect.o AutWriter.o AutParser.o AutPreprocessor.o TuiDisplay.o GridDisplay.o GolSimulator.o StateColor.o -lncurses

sim-gui: sim-gui.cpp GridView.cpp GridView.h ControlDialog.cpp ControlDialog.h GameRules.h makefileqt AutParser.o GameGrid.o AutPreprocessor.o #class.o ensures we have class's dependencies before making
	make -f makefileqt

makefileqt: sim-gui.pro
	qmake sim-gui.pro -o makefileqt

sim-gui.pro: sim-gui.cpp GridView.cpp GridView.h ControlDialog.cpp ControlDialog.h GameGrid.cpp GameGrid.h GolSimulator.cpp GolSimulator.h
	qmake -project -o sim-gui.pro -nopwd sim-gui.cpp GridView.cpp GridView.h ControlDialog.cpp ControlDialog.h GameGrid.cpp GameGrid.h GolSimulator.cpp GolSimulator.h
	echo "CONFIG += debug" >> sim-gui.pro

showgen.o: showgen.cpp GameGrid.h Rect.h Point.h AutParser.h
	g++ -g -Wall -pedantic -c showgen.cpp

sim-tui.o: sim-tui.cpp GameGrid.h
	g++ -g -Wall -pedantic -c sim-tui.cpp

GameGrid.o: GameGrid.cpp GameGrid.h GridSquare.h Point.h Rect.h AutWriter.h GameRules.h
	g++ -g -Wall -pedantic -c GameGrid.cpp

GridSquare.o: GridSquare.cpp GridSquare.h Point.h
	g++ -g -Wall -pedantic -c GridSquare.cpp

CellState.o: CellState.cpp CellState.h StateColor.h
	g++ -g -Wall -pedantic -c CellState.cpp

Point.o: Point.cpp Point.h
	g++ -g -Wall -pedantic -c Point.cpp

Rect.o: Rect.cpp Rect.h Point.h
	g++ -g -Wall -pedantic -c Rect.cpp

AutPreprocessor.o: AutPreprocessor.cpp AutPreprocessor.h
	g++ -g -Wall -pedantic -c AutPreprocessor.cpp

GolSimulator.o: GolSimulator.cpp GolSimulator.h GameGrid.h
	g++ -g -Wall -pedantic -c GolSimulator.cpp

BbSimulator.o: BbSimulator.cpp BbSimulator.h GameGrid.h
	g++ -g -Wall -pedantic -c BbSimulator.cpp

WwSimulator.o: WwSimulator.cpp WwSimulator.h GameGrid.h
	g++ -g -Wall -pedantic -c WwSimulator.cpp

LaSimulator.o: LaSimulator.cpp LaSimulator.h GameGrid.h
	g++ -g -Wall -pedantic -c LaSimulator.cpp

AutWriter.o: AutWriter.cpp AutWriter.h GameGrid.h GameRules.h
	g++ -g -Wall -pedantic -c AutWriter.cpp

GridDisplay.o: GridDisplay.cpp GridDisplay.h
	g++ -g -Wall -pedantic -c GridDisplay.cpp

TuiDisplay.o: TuiDisplay.cpp TuiDisplay.h GridDisplay.h GolSimulator.h
	g++ -g -Wall -pedantic -c TuiDisplay.cpp

test: TestMain.o AutPreprocessorTest.o AutPreprocessor.o AutParserTest.o AutParser.o Rect.o Point.o GameGrid.o GridSquare.o
	g++ -o test TestMain.o AutPreprocessorTest.o AutPreprocessor.o AutParserTest.o AutParser.o Rect.o Point.o GameGrid.o GridSquare.o -lboost_unit_test_framework

TestMain.o: TestMain.cpp
	g++ -g -Wall -pedantic -c TestMain.cpp

AutPreprocessorTest.o: AutPreprocessorTest.cpp AutPreprocessor.h
	g++ -g -Wall -pedantic -c AutPreprocessorTest.cpp

AutParser.o: AutParser.cpp AutParser.h GameGrid.h GameRules.h
	g++ -g -Wall -pedantic -c AutParser.cpp

AutParserTest.o: AutParserTest.cpp AutParser.h GameGrid.h Rect.h Point.h
	g++ -g -Wall -pedantic -c AutParserTest.cpp

StateColor.o: StateColor.cpp StateColor.h
	g++ -g -Wall -pedantic -c StateColor.cpp

clean:
	rm *.o

project.tar.gz: makefile showgen.cpp AutParser.cpp AutParser.h AutParserTest.cpp AutPreprocessor.cpp AutPreprocessor.h AutPreprocessorTest.cpp GameGrid.cpp GameGrid.h GridSquare.cpp GridSquare.h Point.cpp Point.h Rect.cpp Rect.h TestMain.cpp AutWriter.cpp AutWriter.h CellState.cpp CellState.h ControlDialog.cpp ControlDialog.h GameType.h GolSimulator.cpp GolSimulator.h GridDisplay.cpp GridDisplay.h GridView.cpp GridView.h sim-gui.cpp sim-tui.cpp StateColor.cpp StateColor.h TuiDisplay.cpp TuiDisplay.h
	tar -czf project.tar.gz makefile showgen.cpp AutParser.cpp AutParser.h AutParserTest.cpp AutPreprocessor.cpp AutPreprocessor.h AutPreprocessorTest.cpp GameGrid.cpp GameGrid.h GridSquare.cpp GridSquare.h Point.cpp Point.h Rect.cpp Rect.h TestMain.cpp AutWriter.cpp AutWriter.h CellState.cpp CellState.h ControlDialog.cpp ControlDialog.h GameType.h GolSimulator.cpp GolSimulator.h GridDisplay.cpp GridDisplay.h GridView.cpp GridView.h sim-gui.cpp sim-tui.cpp StateColor.cpp StateColor.h TuiDisplay.cpp TuiDisplay.h
