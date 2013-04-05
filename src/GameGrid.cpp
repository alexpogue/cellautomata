#include "GameGrid.h"
/* TODO: REMOVE. FOR TEST ERROR MESSAGES */
#include <iostream>

GameGrid::GameGrid(const Rect& terrain, const Rect& window) { 
  initialize(terrain, window); 
}

GameGrid::GameGrid(const Rect& terrain) {
  initialize(terrain, terrain);
}

void GameGrid::initialize(const Rect& terrain, const Rect& window) {
  terrainBounds = Rect(terrain);
  windowBounds = Rect(window);
  fillWithZeros();
}

void GameGrid::fillWithZeros() {
  for(unsigned int i = 0; i < getTerrainHeight(); i++) {
    std::vector<GridSquare> cur;
    for(unsigned int j = 0; j < getTerrainWidth(); j++) {
      GridSquare gs = GridSquare(Point(j, i));
      cur.push_back(gs);
    }
    grid.push_back(cur);
  }
}

void GameGrid::printToFile(std::ostream& out, 
  const bool& autOutput) const {
  if(!autOutput) {
    unsigned int currRow = getTerrainHeight() - 1;
    do {
      printRow(currRow, out, autOutput);
      out << "\n";
      --currRow;
    } while(currRow+1 > 0); 
    /* currRows overflows from 0 to UINT_MAX, add 1 to set it back to 0 */
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
  return terrainBounds.getTopRight().getX() 
    - terrainBounds.getBottomLeft().getX() + 1;
}

unsigned int GameGrid::getTerrainHeight() const {
  return terrainBounds.getTopRight().getY()
    - terrainBounds.getBottomLeft().getY() + 1;
}

void GameGrid::setSquare(const Point& p, const bool& alive) {
  unsigned int normalizedX = p.getX() - terrainBounds.getBottomLeft().getX();
  unsigned int normalizedY = p.getY() - terrainBounds.getBottomLeft().getY();
  if(normalizedY >= getTerrainWidth() || normalizedX >= getTerrainHeight()) {
    std::cerr << "Tried to set out of bounds GridSquare at (" << p.getX() << ", " << p.getY() << ")\n";
    return;
  }
  grid[normalizedY][normalizedX].setAlive(alive);
}

bool GameGrid::isSquareAlive(const Point& p) const {
  return grid[p.getY()][p.getX()].isAlive();
}
