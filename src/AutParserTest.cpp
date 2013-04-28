#include <boost/test/unit_test.hpp>
#include <string>
#include <vector>
#include "AutParser.h"
#include "GameGrid.h"
#include "Point.h"
#include "Rect.h"
#include <iostream>

void checkGridsEqual(GameGrid &expected, GameGrid &test) {
  BOOST_CHECK_EQUAL(expected.getTerrainBounds.getHeight(), test.getTerrainBounds.getHeight());
  BOOST_CHECK_EQUAL(expected.getTerrainBounds.getWidth(), test.getTerrainBounds.getWidth());
  BOOST_CHECK_EQUAL(expected.getWindowBounds.getHeight(), test.getWindowBounds.getHeight());
  BOOST_CHECK_EQUAL(expected.getWindowBounds.getWidth(), test.getWindowBounds.getWidth());
  if(expected.getTerrainBounds.getHeight() == test.getTerrainBounds.getHeight()
      && expected.getTerrainBounds.getWidth() == test.getTerrainBounds.getWidth()) {
    for(unsigned int i = expected.getTerrainBounds().getBottomLeft().getY(); i <= expected.getTerrainBounds().getTopRight().getY(); i++) {
      for(unsigned int j = expected.getTerrainBounds().getBottomLeft().getX(); j < expected.getTerrainBounds().getTopRight().getX(); j++) {
        Point p(j, i);
        BOOST_CHECK_EQUAL(expected.isSquareAlive(p), test.isSquareAlive(p));
      }
    }
  }
  else {
    BOOST_ERROR("Terrain is not the same size");
  }
}    

void aliveCellsToGrid(const std::vector<Point>& aliveCells, GameGrid& grid) {
  for(unsigned int i = 0; i < aliveCells.size(); i++) {
    grid.setSquare(aliveCells[i], true);
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
  ParseData pData;
  AutParser::parse(fileContents, pData);
  GameGrid testGrid(pData.terrain);
  aliveCellsToGrid(pData.aliveCells, testGrid);
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
  ParseData pData;
  AutParser::parse(fileContents, pData);
  GameGrid testGrid(pData.terrain);
  aliveCellsToGrid(pData.aliveCells, testGrid);
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
  ParseData pData;
  AutParser::parse(fileContents, pData);
  GameGrid testGrid(pData.terrain);
  aliveCellsToGrid(pData.aliveCells, testGrid);
  checkGridsEqual(expectedGrid, testGrid);
}

BOOST_AUTO_TEST_CASE(manyCommentsFile) {
  GameGrid expectedGrid = getExpectedGrid();
  std::string fileContents = "\
#comment\n\
Xrange#comment\n\
-10#comment\n\
10#comment\n\
;#comment\n\
Yrange#comment\n\
-5#comment\n\
5#comment\n\
;#comment\n\
Initial {#comment\n\
Y#comment\n\
=#comment\n\
2#comment\n\
:#comment\n\
-1#comment\n\
;#comment\n\
Y#comment\n\
=#comment\n\
1 : -2, 1;#comment\n\
Y = -1 : 2, 3, 4;#comment\n\
};#comment\n\
#comment";
  ParseData pData;
  AutParser::parse(fileContents, pData);
  GameGrid testGrid(pData.terrain);
  aliveCellsToGrid(pData.aliveCells, testGrid);
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
  ParseData pData;
  AutParser::parse(fileContents, pData);
  GameGrid testGrid(pData.terrain);
  aliveCellsToGrid(pData.aliveCells, testGrid);
  checkGridsEqual(expectedGrid, testGrid);
}
