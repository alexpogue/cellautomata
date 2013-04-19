#include "GolSimulator.h"
/* TODO: REMOVE IOSTREAM */
#include <iostream>

GameGrid simulateGeneration(const GameGrid& gg);
char getNumAliveNeighbors(const GameGrid& gg, Point loc);

GameGrid GolSimulator::simulate(const GameGrid& gg, const unsigned int& numGenerations) {
  GameGrid ret(gg);
  for(unsigned int i = 0; i < numGenerations; i++) {
    ret = simulateGeneration(ret);
  }
  return ret;
}

GameGrid simulateGeneration(const GameGrid& gg) {
  GameGrid dest(gg);
  for(int i = dest.getTerrainBounds().getBottomLeft().getY(); i <= dest.getTerrainBounds().getTopRight().getY(); i++) {
    for(int j = dest.getTerrainBounds().getBottomLeft().getX(); j <= dest.getTerrainBounds().getTopRight().getX(); j++) {
      Point p = Point(j, i);
      char numAlive = getNumAliveNeighbors(gg, p);
      if(gg.isSquareAlive(p)) {
        if(numAlive == 2 || numAlive == 3) {
          dest.setSquare(p, true);
        }
        else if(numAlive < 2 || numAlive > 3) {
          dest.setSquare(p, false);
        }
      }
      else {
        if(numAlive == 3) {
          dest.setSquare(p, true);
        }
      }
    }
  }
  return dest;
}

char getNumAliveNeighbors(const GameGrid& grid, Point loc) {
  char num = 0;
  for(int i = loc.getY() - 1; i <= loc.getY() + 1; i++) {
    for(int j = loc.getX() - 1; j <= loc.getX() + 1; j++) {
      Point check(j, i);
      if(grid.isInBounds(check) && grid.isSquareAlive(check)) {
        ++num;
      }
      else if(!grid.isInBounds(check)) {
      }
      else {
      }
    }
  }
  if(grid.isSquareAlive(loc)) {
    --num;
  }
  return num;
}
