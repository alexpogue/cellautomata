#include "GolSimulator.h"
/* TODO: REMOVE IOSTREAM */
#include <iostream>

GameGrid simulateGolGeneration(const GameGrid& gg);
char getNumAliveNeighbors(const GameGrid& gg, Point loc);

GameGrid GolSimulator::simulate(const GameGrid& gg, const unsigned int& numGenerations) {
  GameGrid ret(gg);
  for(unsigned int i = 0; i < numGenerations; i++) {
    ret = simulateGolGeneration(ret);
  }
  return ret;
}

GameGrid simulateGolGeneration(const GameGrid& gg) {
  GameGrid dest(gg);
  for(int i = dest.getTerrainBounds().getBottomLeft().getY(); i <= dest.getTerrainBounds().getTopRight().getY(); i++) {
    for(int j = dest.getTerrainBounds().getBottomLeft().getX(); j <= dest.getTerrainBounds().getTopRight().getX(); j++) {
      Point p = Point(j, i);
      char numAlive = getNumAliveNeighbors(gg, p);
      if(gg.getSquareState(p).getNum() == 1) {
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
      if(grid.isInBounds(check) && grid.getSquareState(check).getNum() == 1) {
        ++num;
      }
    }
  }
  if(grid.getSquareState(loc).getNum() == 1) {
    --num;
  }
  return num;
}
