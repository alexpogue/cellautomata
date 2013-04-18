#include "GameGrid.h"
/* TODO: REMOVE. FOR TEST ERROR MESSAGES */
#include <iostream>

GameGrid::GameGrid() {
  initialize(Rect(), Rect());
}

GameGrid::GameGrid(const Rect& terrain) {
  initialize(terrain, terrain);
}

GameGrid::GameGrid(const Rect& terrain, const Rect& window) { 
  initialize(terrain, window); 
}

void GameGrid::initialize(const Rect& terrain, const Rect& window) {
  terrainBounds = Rect(terrain);
  windowBounds = Rect(window);
  resetGrid();
}

void GameGrid::resetGrid() {
  grid.resize(getTerrainHeight());
  for(unsigned int i = 0; i < getTerrainHeight(); i++) {
    std::vector<GridSquare> cur;
    cur.resize(getTerrainWidth());
    for(unsigned int j = 0; j < getTerrainWidth(); j++) {
      GridSquare gs;
      cur[j] = gs;
    }
    grid[i] = cur;
  }
}

void GameGrid::printToFile(std::ostream& out, 
  const bool& autOutput) const {
  if(!autOutput) {
    for(int i = windowBounds.getTopRight().getY(); i >= windowBounds.getBottomLeft().getY(); i--) {
      /* this prints the last row first (see GameGrid.h header comments) */
      printRow(i, out);
      out << "\n";
    }
  }
}

void GameGrid::printRow(const int& row, std::ostream& out) const {
  for(int col = windowBounds.getBottomLeft().getX(); col <= windowBounds.getTopRight().getX(); col++) {
    unsigned int serialRow, serialCol;
    serializePoint(serialCol, serialRow, Point(col, row));
    if(isInBounds(Point(col, row)) && grid[serialRow][serialCol].isAlive()) {
      out << "1";
    }
    else {
      out << "~";
    }
  }
}

unsigned int GameGrid::getTerrainWidth() const {
  return terrainBounds.getWidth();
}

unsigned int GameGrid::getTerrainHeight() const {
  return terrainBounds.getHeight();
}

unsigned int GameGrid::getWindowHeight() const {
  return windowBounds.getHeight();
}

unsigned int GameGrid::getWindowWidth() const {
  return windowBounds.getWidth();
}

void GameGrid::setWindowBounds(const Rect& bounds) {
  windowBounds = bounds;
}

void GameGrid::setTerrainBounds(const Rect& bounds) {
  int deltaRowsTop = bounds.getTopRight().getY() - terrainBounds.getTopRight().getY();
  int deltaRowsBottom = terrainBounds.getBottomLeft().getY() - bounds.getBottomLeft().getY();
  int deltaColsLeft = terrainBounds.getBottomLeft().getX() - bounds.getBottomLeft().getX();
  int deltaColsRight = bounds.getTopRight().getX() - terrainBounds.getTopRight().getX();
  
  changeRowsTop(deltaRowsTop);
  changeRowsBottom(deltaRowsBottom);
  changeColsRight(deltaColsRight);
  changeColsLeft(deltaColsLeft);
}

void GameGrid::changeRowsTop(const int& delta) {
  if(delta > 0) {
    addRowsTop(delta);
  }
  else if(delta < 0) {
    removeRowsTop(-delta);
  }
}

void GameGrid::changeRowsBottom(const int& delta) {
  if(delta > 0) {
    addRowsBottom(delta);
  }
  else if(delta < 0) {
    removeRowsBottom(-delta);
  }
}

void GameGrid::changeColsRight(const int& delta) {
  if(delta > 0) {
    addColsRight(delta);
  }
  else if(delta < 0) {
    removeColsRight(-delta);
  }
}

void GameGrid::changeColsLeft(const int& delta) {
  if(delta > 0) {
    addColsLeft(delta);
  }
  else if(delta < 0) {
    removeColsLeft(-delta);
  }
}

void GameGrid::addRowsTop(const unsigned int& numRows) {
  grid.insert(grid.end(), numRows, std::vector<GridSquare>(getTerrainWidth()));
  terrainBounds.setTopRight(Point(terrainBounds.getTopRight().getX(), terrainBounds.getTopRight().getY() + numRows));
}

void GameGrid::addRowsBottom(const unsigned int& numRows) {
  grid.insert(grid.begin(), numRows, std::vector<GridSquare>(getTerrainWidth()));
  terrainBounds.setBottomLeft(Point(terrainBounds.getBottomLeft().getX(), terrainBounds.getBottomLeft().getY() - numRows));
}

void GameGrid::addColsRight(const unsigned int& numCols) {
  for(unsigned int i = 0; i < getTerrainHeight(); i++) {
    grid[i].insert(grid[i].end(), numCols, GridSquare());
  }
  terrainBounds.setTopRight(Point(terrainBounds.getTopRight().getX() + numCols, terrainBounds.getTopRight().getY()));
}

void GameGrid::addColsLeft(const unsigned int& numCols) {
  for(unsigned int i = 0; i < getTerrainHeight(); i++) {
    grid[i].insert(grid[i].begin(), numCols, GridSquare());
  }
  terrainBounds.setBottomLeft(Point(terrainBounds.getBottomLeft().getX() - numCols, terrainBounds.getBottomLeft().getY()));
}

void GameGrid::removeRowsTop(const unsigned int& numRows) {
  grid.erase(grid.end() - numRows, grid.end());
  terrainBounds.setTopRight(Point(terrainBounds.getTopRight().getX(), terrainBounds.getTopRight().getY() - numRows));
}

void GameGrid::removeRowsBottom(const unsigned int& numRows) {
  grid.erase(grid.begin(), grid.begin() + numRows);
  terrainBounds.setBottomLeft(Point(terrainBounds.getBottomLeft().getX(), terrainBounds.getBottomLeft().getY() + numRows));
}

void GameGrid::removeColsRight(const unsigned int& numCols) {
  for(unsigned int i = 0; i < getTerrainHeight(); i++) {
    grid[i].erase(grid[i].end() - numCols, grid[i].end());
  }
  terrainBounds.setTopRight(Point(terrainBounds.getTopRight().getX() - numCols, terrainBounds.getTopRight().getY()));
}

void GameGrid::removeColsLeft(const unsigned int& numCols) {
  for(unsigned int i = 0; i < getTerrainHeight(); i++) {
    grid[i].erase(grid[i].begin(), grid[i].begin() + numCols);
  }
  terrainBounds.setBottomLeft(Point(terrainBounds.getBottomLeft().getX() + numCols, terrainBounds.getBottomLeft().getY()));
}

void GameGrid::setSquare(const Point& p, const bool& alive) {
  if(!isInBounds(p)) {
    std::cerr << "Tried to set out of bounds GridSquare at " << p << "\n";
    return;
  }
  unsigned int serialX, serialY;
  serializePoint(serialX, serialY, p);
  grid[serialY][serialX].setAlive(alive);
}

bool GameGrid::isInBounds(const Point& p) const {
  return p.getX() >= terrainBounds.getBottomLeft().getX() && p.getX() <= terrainBounds.getTopRight().getX()
    && p.getY() >= terrainBounds.getBottomLeft().getY() && p.getY() <= terrainBounds.getTopRight().getY(); 
}

void GameGrid::serializePoint(unsigned int& serialX, unsigned int& serialY, const Point& p) const {
  serialX = p.getX() - terrainBounds.getBottomLeft().getX();
  serialY = p.getY() - terrainBounds.getBottomLeft().getY();
}

void GameGrid::normalizePoint(Point& p, const unsigned int& serialX, const unsigned int& serialY) const {
  int normX = serialX + terrainBounds.getBottomLeft().getX();
  int normY = serialY + terrainBounds.getBottomLeft().getY();
  p.setX(normX);
  p.setY(normY);
}

bool GameGrid::isSquareAlive(const Point& p) const {
  return grid[p.getY()][p.getX()].isAlive();
}
