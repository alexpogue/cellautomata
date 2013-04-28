#include "GameGrid.h"
/* TODO: REMOVE. FOR TEST ERROR MESSAGES */
#include <iostream>
#include "AutWriter.h"

GameGrid::GameGrid() {
  initialize(Rect(), Rect(), std::vector<CellState>());
}

GameGrid::GameGrid(const Rect& terrain) {
  initialize(terrain, terrain, std::vector<CellState>());
}

GameGrid::GameGrid(const Rect& terrain, const Rect& window) { 
  initialize(terrain, window, std::vector<CellState>()); 
}

GameGrid::GameGrid(const Rect& terrain, const std::vector<CellState>& states) {
  initialize(terrain, terrain, states);
}

GameGrid::GameGrid(const Rect& terrain, const Rect& window, const std::vector<CellState>& states) {
  initialize(terrain, window, states);
}

void GameGrid::initialize(const Rect& terrain, const Rect& window, const std::vector<CellState>& states) {
  terrainBounds = Rect(terrain);
  windowBounds = Rect(window);
  gameStates = std::vector<CellState>(states);
  if(gameStates.size() == 0) {
    gameStates.push_back(CellState(0, '~', StateColor()));
  }
  resetGrid();
}

void GameGrid::resetGrid() {
  grid.resize(getTerrainBounds().getHeight());
  for(unsigned int i = 0; i < getTerrainBounds().getHeight(); i++) {
    std::vector<GridSquare> cur;
    cur.resize(getTerrainBounds().getWidth());
    for(unsigned int j = 0; j < getTerrainBounds().getWidth(); j++) {
      GridSquare gs(gameStates[0]);
      cur[j] = gs;
    }
    grid[i] = cur;
  }
}

void GameGrid::setName(const std::string& n) {
  name = n;
}

std::string GameGrid::getName() const {
  return name;
}

void GameGrid::printToFile(std::ostream& out, 
  const bool& autOutput) const {
  if(autOutput) {
    AutWriter::write(*this, out);
  }
  else {
    for(int i = getWindowBounds().getTopRight().getY(); i >= getWindowBounds().getBottomLeft().getY(); i--) {
      /* this prints the last row first (see GameGrid.h header comments) */
      printRow(i, out);
      out << "\n";
    }
  }
}

std::string GameGrid::getAsciiString() {
  std::string ret;
  for(int i = getWindowBounds().getTopRight().getY(); i >= getWindowBounds().getBottomLeft().getY(); i--) {
    /* this prints the last row first (see GameGrid.h header comments) */
    ret += getRowAsciiString(i);
    ret += "\n";
  }
  return ret;
}

std::string GameGrid::getRowAsciiString(int row) {
  std::string ret;
  for(int col = getWindowBounds().getBottomLeft().getX(); col <= getWindowBounds().getTopRight().getX(); col++) {
    if(isInBounds(Point(col, row))) {
      unsigned int serialRow, serialCol;
      serializePoint(serialCol, serialRow, Point(col, row));
      ret += grid[serialRow][serialCol].getState().getChar();
    }
    else {
      ret += gameStates[0].getChar();
    }
  }
  return ret;
}


void GameGrid::printRow(const int& row, std::ostream& out) const {
  for(int col = getWindowBounds().getBottomLeft().getX(); col <= getWindowBounds().getTopRight().getX(); col++) {
    if(isInBounds(Point(col, row))) {
      unsigned int serialRow, serialCol;
      serializePoint(serialCol, serialRow, Point(col, row));
      out << grid[serialRow][serialCol].getState().getChar();
    }
    else {
      out << gameStates[0].getChar();
    }
  }
}

Rect GameGrid::getTerrainBounds() const {
  return terrainBounds;
}

Rect GameGrid::getWindowBounds() const {
  return windowBounds;
}

void GameGrid::setWindowBounds(const Rect& bounds) {
  windowBounds = bounds;
}

void GameGrid::setTerrainBounds(const Rect& bounds) {
  int deltaRowsTop = bounds.getTopRight().getY() - getTerrainBounds().getTopRight().getY();
  int deltaRowsBottom = getTerrainBounds().getBottomLeft().getY() - bounds.getBottomLeft().getY();
  int deltaColsLeft = getTerrainBounds().getBottomLeft().getX() - bounds.getBottomLeft().getX();
  int deltaColsRight = bounds.getTopRight().getX() - getTerrainBounds().getTopRight().getX();
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
  grid.insert(grid.end(), numRows, std::vector<GridSquare>(getTerrainBounds().getWidth(), GridSquare(gameStates[0])));
  terrainBounds.setTopRight(Point(getTerrainBounds().getTopRight().getX(), getTerrainBounds().getTopRight().getY() + numRows));
}

void GameGrid::addRowsBottom(const unsigned int& numRows) {
  grid.insert(grid.begin(), numRows, std::vector<GridSquare>(getTerrainBounds().getWidth(), GridSquare(gameStates[0])));
  terrainBounds.setBottomLeft(Point(getTerrainBounds().getBottomLeft().getX(), getTerrainBounds().getBottomLeft().getY() - numRows));
}

void GameGrid::addColsRight(const unsigned int& numCols) {
  for(unsigned int i = 0; i < getTerrainBounds().getHeight(); i++) {
    grid[i].insert(grid[i].end(), numCols, GridSquare(gameStates[0]));
  }
  terrainBounds.setTopRight(Point(getTerrainBounds().getTopRight().getX() + numCols, getTerrainBounds().getTopRight().getY()));
}

void GameGrid::addColsLeft(const unsigned int& numCols) {
  for(unsigned int i = 0; i < getTerrainBounds().getHeight(); i++) {
    grid[i].insert(grid[i].begin(), numCols, GridSquare(gameStates[0]));
  }
  terrainBounds.setBottomLeft(Point(getTerrainBounds().getBottomLeft().getX() - numCols, getTerrainBounds().getBottomLeft().getY()));
}

void GameGrid::removeRowsTop(const unsigned int& numRows) {
  grid.erase(grid.end() - numRows, grid.end());
  terrainBounds.setTopRight(Point(getTerrainBounds().getTopRight().getX(), getTerrainBounds().getTopRight().getY() - numRows));
}

void GameGrid::removeRowsBottom(const unsigned int& numRows) {
  grid.erase(grid.begin(), grid.begin() + numRows);
  terrainBounds.setBottomLeft(Point(getTerrainBounds().getBottomLeft().getX(), getTerrainBounds().getBottomLeft().getY() + numRows));
}

void GameGrid::removeColsRight(const unsigned int& numCols) {
  for(unsigned int i = 0; i < getTerrainBounds().getHeight(); i++) {
    grid[i].erase(grid[i].end() - numCols, grid[i].end());
  }
  terrainBounds.setTopRight(Point(getTerrainBounds().getTopRight().getX() - numCols, getTerrainBounds().getTopRight().getY()));
}

void GameGrid::removeColsLeft(const unsigned int& numCols) {
  for(unsigned int i = 0; i < getTerrainBounds().getHeight(); i++) {
    grid[i].erase(grid[i].begin(), grid[i].begin() + numCols);
  }
  terrainBounds.setBottomLeft(Point(getTerrainBounds().getBottomLeft().getX() + numCols, getTerrainBounds().getBottomLeft().getY()));
}

void GameGrid::setSquare(const Point& p, int stateNum) {
  if(!isInBounds(p)) {
    std::cerr << "Tried to set out of bounds GridSquare at " << p << "\n";
    return;
  }
  unsigned int serialX, serialY;
  serializePoint(serialX, serialY, p);
  grid[serialY][serialX].setState(gameStates[stateNum]);
}

bool GameGrid::isInBounds(const Point& p) const {
  return p.getX() >= getTerrainBounds().getBottomLeft().getX() && p.getX() <= getTerrainBounds().getTopRight().getX()
    && p.getY() >= getTerrainBounds().getBottomLeft().getY() && p.getY() <= getTerrainBounds().getTopRight().getY(); 
}

void GameGrid::serializePoint(unsigned int& serialX, unsigned int& serialY, const Point& p) const {
  serialX = p.getX() - getTerrainBounds().getBottomLeft().getX();
  serialY = p.getY() - getTerrainBounds().getBottomLeft().getY();
}

void GameGrid::normalizePoint(Point& p, const unsigned int& serialX, const unsigned int& serialY) const {
  int normX = serialX + getTerrainBounds().getBottomLeft().getX();
  int normY = serialY + getTerrainBounds().getBottomLeft().getY();
  p.setX(normX);
  p.setY(normY);
}

CellState GameGrid::getSquareState(const Point& p) const {
  unsigned int serialX, serialY;
  serializePoint(serialX, serialY, p);
  return grid[serialY][serialX].getState();
}

void GameGrid::setGameStates(const std::vector<CellState>& states) {
  gameStates = std::vector<CellState>(states);
  refillStates();
}

void GameGrid::setGameStates(const std::string& chars) {
  for(unsigned int i = 0; i < chars.length(); i++) {
    CellState toSet(i, chars[i], StateColor());
    if(i >= gameStates.size()) {
      gameStates.push_back(toSet);
    }
    else {
      gameStates[i] = toSet;
    }
  }
  refillStates();
}

void GameGrid::refillStates() {
  for(int i = getTerrainBounds().getBottomLeft().getX(); i <= getTerrainBounds().getTopRight().getX(); i++) {
    for(int j = getTerrainBounds().getBottomLeft().getY(); j <= getTerrainBounds().getTopRight().getY(); j++) {
      setSquare(Point(i, j), getSquareState(Point(i, j)).getNum());
    }
  }
}
