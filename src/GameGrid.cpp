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
      int xPos = j + terrainBounds.getBottomLeft().getX();
      int yPos = i + terrainBounds.getBottomLeft().getY();
      GridSquare gs(Point(xPos, yPos));
      cur[j] = gs;
    }
    grid[i] = cur;
  }
}

void GameGrid::printToFile(std::ostream& out, 
  const bool& autOutput) const {
  if(!autOutput) {
    for(unsigned int i = 0; i < getTerrainHeight(); i++) {
      /* this prints the last row first (see GameGrid.h header comments) */
      printRow(getTerrainHeight() - 1 - i, out, autOutput);
      out << "\n";
    }
  }
}

void GameGrid::printRow(const int& row, std::ostream& out, 
  const bool& autOutput) const {
  for(unsigned int col = 0; col < getTerrainWidth(); col++) {
    if(grid[row][col].isAlive()) {
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
  for(unsigned int i = 0; i < numRows; i++) {
    std::vector<GridSquare> newRow;
    newRow.resize(getTerrainWidth());
    for(unsigned int j = 0; j < getTerrainWidth(); j++) {
      GridSquare gs(Point(j, i));
      newRow[j] = gs;
    }
    grid.push_back(newRow);
  }
  terrainBounds.setTopRight(Point(terrainBounds.getTopRight().getX(), terrainBounds.getTopRight().getY() + numRows));
}

void GameGrid::addRowsBottom(const unsigned int& numRows) {
  for(unsigned int i = 0; i < numRows; i++) {
    std::vector<GridSquare> newRow;
    newRow.resize(getTerrainWidth());
    for(unsigned int j = 0; j < getTerrainWidth(); j++) {
      GridSquare gs(Point(j, i + 1 + getTerrainHeight()));
      newRow[j] = gs;
    }
    std::vector<std::vector<GridSquare> >::iterator it;
    it = grid.begin();
    grid.insert(it, newRow);
  }
  terrainBounds.setBottomLeft(Point(terrainBounds.getBottomLeft().getX(), terrainBounds.getBottomLeft().getY() - numRows));
}

void GameGrid::addColsRight(const unsigned int& numCols) {
  for(unsigned int i = 0; i < numCols; i++) {
    for(unsigned int j = 0; j < getTerrainHeight(); j++) {
      Point p;
      normalizePoint(p, i, j);
      GridSquare gs(p);
      grid[j].push_back(gs);
    }
  }
  terrainBounds.setTopRight(Point(terrainBounds.getTopRight().getX() + numCols, terrainBounds.getTopRight().getY()));
}

void GameGrid::addColsLeft(const unsigned int& numCols) {
  for(unsigned int i = 0; i < numCols; i++) {
    for(unsigned int j = 0; j < getTerrainHeight(); j++) {
      Point p;
      normalizePoint(p, i, j);
      GridSquare gs(p);
      std::vector<GridSquare>::iterator it;
      it = grid[j].begin();
      grid[j].insert(it, gs);
    }
  }
  terrainBounds.setBottomLeft(Point(terrainBounds.getBottomLeft().getX() - numCols, terrainBounds.getBottomLeft().getY()));
}

void GameGrid::removeRowsTop(const unsigned int& numRows) {
}

void GameGrid::removeRowsBottom(const unsigned int& numRows) {
  grid.erase(grid.begin(), grid.begin() + numRows);
}

void GameGrid::removeColsRight(const unsigned int& numCols) {
}

void GameGrid::removeColsLeft(const unsigned int& numCols) {
}

void GameGrid::setSquare(const Point& p, const bool& alive) {
  unsigned int normX, normY;
  serializePoint(normX, normY, p);
  if(normY >= getTerrainHeight() || normX >= getTerrainWidth()) {
    std::cerr << "Tried to set out of bounds GridSquare at (" << p.getX() << ", " << p.getY() << ")\n";
    return;
  }
  grid[normY][normX].setAlive(alive);
}

void GameGrid::serializePoint(unsigned int& normX, unsigned int& normY, const Point& p) {
  normX = p.getX() - terrainBounds.getBottomLeft().getX();
  normY = p.getY() - terrainBounds.getBottomLeft().getY();
}

void GameGrid::normalizePoint(Point& pSerial, const unsigned int& normX, const unsigned int& normY) {
  int serialX = normX + terrainBounds.getBottomLeft().getX();
  int serialY = normY + terrainBounds.getBottomLeft().getY();
  pSerial.setX(serialX);
  pSerial.setY(serialY);
}

bool GameGrid::isSquareAlive(const Point& p) const {
  return grid[p.getY()][p.getX()].isAlive();
}
