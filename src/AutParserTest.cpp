#include <boost/test/unit_test.hpp>
#include <string>
#include "AutParser.h"
#include "GameGrid.h"
#include "Point.h"
#include "Rect.h"

void checkGridsEqual(GameGrid &expected, GameGrid &test) {
  BOOST_CHECK_EQUAL(expected.getTerrainHeight(), test.getTerrainHeight());
  BOOST_CHECK_EQUAL(expected.getTerrainWidth(), test.getTerrainWidth());
  BOOST_CHECK_EQUAL(expected.getWindowHeight(), test.getWindowHeight());
  BOOST_CHECK_EQUAL(expected.getWindowWidth(), test.getWindowWidth());
  for(unsigned int i = 0; i < expected.getTerrainHeight(); i++) {
    for(unsigned int j = 0; j < expected.getTerrainWidth(); j++) {
      Point p(j, i);
      BOOST_CHECK_EQUAL(expected.isSquareAlive(p), test.isSquareAlive(p));
    }
  }
}    

GameGrid getExpectedGrid() {
  Rect terrainBounds(Point(-10, -5), Point(10, 5));
  GameGrid expectedGrid(terrainBounds);
  expectedGrid.setSquare(Point(-1,2), true);
  expectedGrid.setSquare(Point(-2,1), true);
  expectedGrid.setSquare(Point(1,1), true);
  expectedGrid.setSquare(Point(2,-1), true);
  expectedGrid.setSquare(Point(3,-1), true);
  expectedGrid.setSquare(Point(4,-1), true);
  return expectedGrid;
}

BOOST_AUTO_TEST_CASE(simpleFile) {
  GameGrid expectedGrid = getExpectedGrid();
  std::string fileContents = "\
Xrange -10 10;\n\
Yrange -5 5;\n\
Initial {\n\
Y = 2 : -1;\n\
Y = 1 : -2, 1;\n\
Y = -1 : 2, 3, 4;\n\
};\n";
  GameGrid testGrid(AutParser::parse(fileContents));
  checkGridsEqual(expectedGrid, testGrid);
}

BOOST_AUTO_TEST_CASE(varySpacesInInitialFile) {
  GameGrid expectedGrid = getExpectedGrid();
  std::string fileContents = "\
Xrange -10 10;\n\
Yrange -5 5;\n\
Initial {\n\
Y= 2 : -1;\n\
Y =1 : -2, 1;\n\
Y = -1:2,3, 4;\n\
};\n";
  GameGrid testGrid(AutParser::parse(fileContents));
  checkGridsEqual(expectedGrid, testGrid);
}

BOOST_AUTO_TEST_CASE(commentsFile) {
  GameGrid expectedGrid = getExpectedGrid();
  std::string fileContents = "\
#comment\n\
Xrange -10 10;#comment\n\
Yrange -5 5;#comment\n\
Initial {#comment\n\
Y = 2 : -1;#comment\n\
Y = 1 : -2, 1;#comment\n\
Y = -1 : 2, 3, 4;#comment\n\
};#comment\n\
#comment";
  GameGrid testGrid(AutParser::parse(fileContents));
  checkGridsEqual(expectedGrid, testGrid);
}

BOOST_AUTO_TEST_CASE(strangeFormatting) {
  GameGrid expectedGrid = getExpectedGrid();
  std::string fileContents = "\
\n\
Xrange \n\
-10 \n\
10;\n\
Yrange -5 \n\
5;\n\
Initial \n\
{\n\
Y \n\
= \n\
2 \n\
: \n\
-1\n\
;\n\
Y = 1 \n\
: -2, 1;\n\
Y = -1 : 2, 3, 4;\n\
};\n";
  GameGrid testGrid(AutParser::parse(fileContents));
  checkGridsEqual(expectedGrid, testGrid);
}
